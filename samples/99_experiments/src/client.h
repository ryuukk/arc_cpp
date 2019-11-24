#include <utility>

#pragma once

enum class State
{
    None,
    Connecting,
    Connected,
    Disconnected
};

enum class EventType
{
    None,
    Error,
    Connected,
    Disconnected,
    Data,
    Packet,
    Listenning,
    Stopped
};


struct Message
{
    long connectionId{};
    EventType eventType{};
    IPacket* packet = nullptr;

    ~Message()
    {
        delete packet;
    }
};

class NetClient
{
private:

    volatile State _state = State::Disconnected;
    std::thread* _thread = nullptr;
    std::queue<Message*> _msgQueue{};
    net::socket _sock;
    std::string _host;
    int _port{};
    std::function<IPacket*(unsigned char)> _packetMapCb{};
    RC4 _cipherSend;
    RC4 _cipherReceive;
    std::vector<unsigned char> _bufferSend{};

public:

    NetClient(const std::vector<unsigned char>& key1, const std::vector<unsigned char>& key2)
    {
        _cipherReceive.setKey(key2);
        _cipherSend.setKey(key1);
        _bufferSend.resize(1024 * 10);
    }

    void registerTypes(const std::function<IPacket*(unsigned char)>& cb)
    {
        _packetMapCb = std::move(cb);
    }

    void connect(const std::string& host, int port)
    {
        if(_state != State::Disconnected) return;

        _host = host;
        _port = port;
        if(_thread != nullptr)
        {
            delete _thread;
            _thread = nullptr;
        }

        _thread = new std::thread(&NetClient::clientLoop, std::ref(*this));
        _state = State::Connected;
    }

    void disconnect()
    {
        if (_state == State::Disconnected) return;
        _state = State::Disconnected;

        _sock.shutdown(net::shut::rdwr);
        _sock.close();

        auto msg = new Message();
        msg->eventType = EventType::Disconnected;
        _msgQueue.push(msg);
    }

    bool hasMessages()
    {
        return !_msgQueue.empty();
    }

    Message* getNextMessage()
    {
        if(_msgQueue.empty())
            return nullptr;

        auto* ret = _msgQueue.front();
        _msgQueue.pop();
        return ret;
    }

    void sendPacket(IPacket& packet)
    {
        if (_state != State::Connected) return;

        auto writer = PWriter(_bufferSend);
        packet.write(writer);

        auto data = writer.getRange();
        _cipherSend.cypher(data, 0);

        auto packetLength = data.length() + 5;

        std::vector<unsigned char> fnl;
        fnl.resize(packetLength);
        fnl[0] = (unsigned char) (packetLength >> 24);
        fnl[1] = (unsigned char) (packetLength >> 16);
        fnl[2] = (unsigned char) (packetLength >> 8);
        fnl[3] = (unsigned char) (packetLength);


        fnl[4] = packet.id();
        if(data.length() > 0)
            memcpy(&fnl[5], data.begin(), data.length());

        auto bytesSent = _sock.send(fnl);

        assert(bytesSent == packetLength && "packet not fully sent");
    }

    void sendPacket(std::vector<unsigned char>& data)
    {
        if (_state != State::Connected) return;
        auto bytesSent = _sock.send(data);

        assert(bytesSent == data.size() && "packet not fully sent");
    }

private:

    static arc::Span<unsigned char> readBytes(
            std::vector<unsigned char>& buffer,
            net::socket& socket,
            int count, bool& closed)
    {
        int numRead = 0;

        while (count > 0)
        {
            auto n = (int) socket.recv(&buffer[numRead], count);
            if (n == -1)
            {
                printf("-1 bro\n");
                closed = true;
                break;
            }
            if (n == 0)
            {
                printf("0 bro\n");
                closed = true;
                break;
            }

            numRead += n;
            count -= n;
            //printf("READ: %d\n", n);
        }

        return {&buffer[0], numRead};
    }
    static void clientLoop(NetClient& client)
    {
        // try connect
        client._sock = net::socket( net::af::inet, net::sock::stream);

        auto connectResult = client._sock.connect({client._host, client._port});

        if(connectResult == -1)
        {
            // error
            arc::log::errorf("Unnable to connect to: {0}:{1} error:{2}", client._host, client._port, connectResult);
            return;
        }

        client._state = State::Connected;

        auto msg = new Message();
        msg->eventType = EventType::Connected;
        client._msgQueue.push(msg);

        auto buffer = std::vector<unsigned char>();
        buffer.resize(1024 * 50);

        while (client._state == State::Connected)
        {
            auto closed = false;
            auto packetLengthSpan = readBytes(buffer, client._sock, 4, closed);
            if(closed)
            {
                client.disconnect();
                break;
            }

            auto packetLength = (int) packetLengthSpan[0] << 24 |
                                (int) packetLengthSpan[1] << 16 |
                                (int) packetLengthSpan[2] << 8 |
                                (int) packetLengthSpan[3];


            //printf("Packet Lenght: %i\n", packetLength);

            auto packetData = readBytes(buffer, client._sock, packetLength - 4, closed);

            if(closed)
            {
                client.disconnect();
                break;
            }

            auto packetId = packetData[0];

            // todo: need to figure out how to slice a span, then i can remove the offset, and the readByte
            client._cipherReceive.cypher(packetData, 1);

            auto packet = client._packetMapCb(packetId);
            if(packet)
            {
                PReader reader = PReader(packetData);
                reader.readByte();
                packet->read(reader);

                auto msgPacket = new Message();
                msgPacket->eventType = EventType::Packet;
                msgPacket->packet = packet;
                client._msgQueue.push(msgPacket);
            } else
            {
                arc::log::errorf("Packet ID: {0} with L: {1} not supported", (int) packetId, packetLength);
            }
        }

        printf("End Client loop\n");
    }

};
