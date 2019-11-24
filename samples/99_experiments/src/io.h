#pragma once

#include <utils/Span.h>
#include <string>

struct RC4
{
    std::vector<unsigned char> key;
    unsigned char state[256];
    int x{};
    int y{};

    void setKey(const std::vector<unsigned char>& k)
    {
        key = k;
        reset();
    }

    void cypher(arc::Span<unsigned char>& bytes, int offset)
    {
        for (int i = offset; i < bytes.length(); i++)
        {
            x = (x + 1) & 0xff;
            y = (state[x] + y) & 0xff;

            //swap
            std::swap(state[x], state[y]);

            //xor
            bytes[i] ^= state[(state[x] + state[y]) & 0xff];
        }
    }

    void reset()
    {
        x = y = 0;

        for (int i = 0; i < 256; i++)
            state[i] = (unsigned char)i;

        int a = 0;
        int b = 0;

        for (int i = 0; i < 256; i++)
        {
            b = ((key[a] & 0xff) + state[i] + b) & 0xff;

            //swap
            std::swap(state[i], state[b]);

            a = (a + 1) % 13; //% keylength
        }
    }
};

struct PReader
{
    arc::Span<unsigned char>& buffer;
    int position;

    explicit PReader(arc::Span<unsigned char>& data): buffer(data), position(0)
    {
    }

    bool readBool()
    {
        return buffer[position++];
    }

    unsigned char readByte()
    {
        return buffer[position++];
    }

    short readShort()
    {
        auto ret = (short) buffer[position + 0] << 8 |
                   (short) buffer[position + 1];
        position += 2;
        return ret;
    }

    int readInt()
    {
        auto ret = (int) buffer[position + 0] << 24 |
                   (int) buffer[position + 1] << 16 |
                   (int) buffer[position + 2] << 8 |
                   (int) buffer[position + 3];
        position += 4;
        return ret;
    }

    unsigned short readUShort()
    {
        auto ret = (unsigned short) buffer[position + 0] << 8 |
                   (unsigned short) buffer[position + 1];
        position += 2;
        return ret;
    }

    unsigned int readUInt()
    {
        auto ret = (unsigned int) buffer[position + 0] << 24 |
                   (unsigned int) buffer[position + 1] << 16 |
                   (unsigned int) buffer[position + 2] << 8 |
                   (unsigned int) buffer[position + 3];
        position += 4;
        return ret;
    }

    float readFloat()
    {
        auto i = readUInt();
        auto ret = *(float*)&i;
        return ret;
    }

    std::string readString()
    {
        auto length = readShort();

        auto ret = std::string(reinterpret_cast<char*>(&buffer[position]), length);
        position += length;
        return ret;
    }

    std::string readUTF32()
    {
        auto length = readInt();

        auto ret = std::string(reinterpret_cast<char*>(&buffer[position]), length);
        position += length;
        return ret;
    }
};

struct PWriter
{
    std::vector<unsigned char>& buffer;
    int position;

    explicit PWriter(std::vector<unsigned char>& data): buffer(data), position(0)
    {
    }

    void writeBool(const bool & data)
    {
        buffer[position++] = (unsigned char) (data ? 1 : 0);
    }

    void writeByte(const unsigned char& data)
    {
        buffer[position++] = data;
    }

    void writeShort(const short& data)
    {
        buffer[position++] = (unsigned char) (data >> 8);
        buffer[position++] = (unsigned char) data;
    }

    void writeInt(const int& data)
    {
        buffer[position++] = (unsigned char) (data >> 24);
        buffer[position++] = (unsigned char) (data >> 16);
        buffer[position++] = (unsigned char) (data >> 8);
        buffer[position++] = (unsigned char) (data);
    }

    void writeUShort(const unsigned short& data)
    {
        buffer[position++] = (unsigned char) (data >> 8);
        buffer[position++] = (unsigned char) data;
    }

    void writeUInt(const unsigned int& data)
    {
        buffer[position++] = (unsigned char) (data >> 24);
        buffer[position++] = (unsigned char) (data >> 16);
        buffer[position++] = (unsigned char) (data >> 8);
        buffer[position++] = (unsigned char) (data);
    }

    void writeFloat(const float& data)
    {
        auto fd = *(unsigned int*)&data;
        buffer[position++] = (unsigned char) (fd >> 24);
        buffer[position++] = (unsigned char) (fd >> 16);
        buffer[position++] = (unsigned char) (fd >> 8);
        buffer[position++] = (unsigned char) (fd);
    }

    void writeString(const std::string& data)
    {
        writeShort((short)data.length());

        memcpy(&buffer[position], data.c_str(), data.length());
        position += data.length();
    }

    void writeUTF32(const std::string& data)
    {
        writeInt((int)data.length());

        memcpy(&buffer[position], data.c_str(), data.length());
        position += data.length();
    }

    arc::Span<unsigned char> getRange()
    {
        return {&buffer[0], position};
    }
};