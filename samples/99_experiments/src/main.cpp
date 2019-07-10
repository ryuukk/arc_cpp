#include <fstream>
#include <string.h>
#include <Core.h>
#include <Configuration.h>
#include <Engine.h>

#include <gfx/Model.h>
#include <gfx/ModelInstance.h>
#include <gfx/data/ModelData.h>
#include <utils/IO.h>

#include <net/tcp/Client.hpp>
#include <net/tcp/utils/Messages.hpp>
#include <net/tcp/utils/Errors.hpp>
#include <net/tcp/utils/Sockets.hpp>
#include <net/tcp/Server.hpp>


class PacketWriter
{
private:
    unsigned char* data;
    int offset;
public:
    PacketWriter(unsigned char* data, int size, unsigned char id): data(data), offset(0)
    {
        write(size);
        write(id);
    }

    void write(const int& val)
    {
        auto* ptr = (unsigned char*)&val;
        data[offset] = ptr[3];
        data[offset + 1] = ptr[2];
        data[offset + 2] = ptr[1];
        data[offset + 3] = ptr[0];
        offset += 4;
    }

    void write(const unsigned char& val)
    {
        data[offset] = val;
        offset++;
    }

    void write(const std::string& val)
    {
        auto* ptr = (unsigned char*) val.length();
        data[offset] = ptr[1];
        data[offset + 1] = ptr[0];

        memcpy(data + offset + 2, val.c_str(), val.length());
        offset += 2 + val.length();
    }
};

class HelloPacket
{
public:
    std::string buildVersion;
    int gameId;
    std::string guid;
    int random1;
    std::string password;
    int random2;
    std::string secret;
    int keyTime;
    std::string  key;
    std::string mapJSON;
    std::string entryTag;
    std::string gameNet;
    std::string gameNetUserId;
    std::string playPlatform;
    std::string platformToken;
    std::string userToken;

    void emplace(unsigned char* buffer)
    {
        PacketWriter w(buffer, size(), 100);

        w.write(buildVersion);
        w.write(gameId);
        w.write(guid);
        w.write(random1);
        w.write(password);
        w.write(random2);
        w.write(secret);
        w.write(keyTime);
        w.write(key);
        w.write(mapJSON);
        w.write(entryTag);
        w.write(gameNet);
        w.write(gameNetUserId);
        w.write(playPlatform);
        w.write(platformToken);
        w.write(userToken);
    };

    int size()
    {
        return 47 + buildVersion.length() + guid.length() + password.length() + secret.length() + key.length() + mapJSON.length()
               + entryTag.length() + gameNet.length() + gameNetUserId.length() + playPlatform.length() + platformToken.length() + userToken.length();
    };
};

class MyGame : public arc::IApp
{

    Network::TCP::Client* client;
    Network::TCP::Server* server;

    void create() override {

        Network::Start();

        server = new Network::TCP::Server();
        server->start(2050);

        client = new Network::TCP::Client();
        if(client->connect("127.0.0.1", 2050))
        {
            arc::Core::logger->info("Connected !");
        }
        else
        {
            arc::Core::logger->error("Unable to connect");
        }
    }

    void update(float dt) override {

        server->update();
        while (auto msg = server->poll())
        {
            if(msg->is<Network::Messages::Connection>())
            {
                auto connection = msg->as<Network::Messages::Connection>();
                if (connection->result == Network::Messages::Connection::Result::Success)
                {
                    arc::Core::logger->info("Server, got client !");

                    // packet:
                    //    i32     length
                    //    i8      id
                    //    byte*   content[length - 5]
                }
                else
                {
                    arc::Core::logger->error("Server, client unable to connect");
                }
            }
        }


        while (auto msg = client->poll())
        {
            if (msg->is<Network::Messages::Connection>())
            {

                auto connection = msg->as<Network::Messages::Connection>();
                if (connection->result == Network::Messages::Connection::Result::Success)
                {
                    arc::Core::logger->info("Connected to server!");

                    // packet:
                    //    i32     length
                    //    i8      id
                    //    byte*   content[length - 5]

                }
                else
                {
                    arc::Core::logger->error("Unable to connect to server");
                }
            }
            else if (msg->is<Network::Messages::Disconnection>())
            {
                arc::Core::logger->error("Disconnected");
            }
            else if (msg->is<Network::Messages::UserData>())
            {
                arc::Core::logger->error("Data!!!");
                auto userdata = msg->as<Network::Messages::UserData>();
            }
        }
    }

    void render(float dt) override {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        glEnable(GL_DEPTH_TEST);

    }

    void resize(int width, int height) override {

    }

    void dispose() override {
        server->stop();

        delete server;
        delete client;

        Network::Release();
    }

};

int main(int argc, char** argv) {
    auto config = arc::Configuration();
    config.windowTitle = "Sample 99 - Experiments";
    auto myGame = new MyGame();
    auto engine = new arc::Engine(myGame, config);
    engine->run();

    return 0;
}