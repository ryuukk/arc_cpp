#define _CRTDBG_MAP_ALLOC

#include <crtdbg.h>
#include <fstream>
#include <string.h>
#include <thread>
#include <functional>

#include <queue>

#include <Core.h>
#include <Configuration.h>

#include <Engine.h>
#include <gfx/Model.h>
#include <gfx/ModelInstance.h>
#include <gfx/data/ModelData.h>
#include <utils/IO.h>
#include <net/Socket.h>
#include <utils/Span.h>
#include <utils/TimeUtils.h>
#include <utils/StopWatch.h>
#include "io.h"
#include "packets.h"
#include "client.h"

class Entity
{
public:
    int entityId{};
    int entityType{};
    float x{};
    float y{};
    float z{};
    std::string name{};

    void parseData(const std::vector<StatData>& data)
    {
        for (const auto & stat : data) {
            if(stat.id == StatsType::NAME_STAT)
                name = stat.stringValue;

        }
    }

    void onNewTick(const arc::Vec2 newPos, int tickTime, int tickId)
    {
        x = newPos.x;
        y = newPos.y;
    }
    void onGoto(float newX, float newY)
    {
        x = newX;
        y = newY;
    }
};

class MyGame : public arc::IApp
{
    const std::string BUILD = "X32.2.1";
    const std::string GUID = "tvSUJQAJWjEHE4ynVBmFum2i5drx8E0PPBjeGWUMcB00ZGTVoY8aPXKXv4R9VX40DlguiAb2+k/wAHEn35s7bilAyrlrhrLn8GxpWA4q3xa3siSlIB9056shXBYBQC18AQTAaYmxjp9OREEqBf4sw7Q3Ggz8RWx3ekWa7LAgIcM=";
    const std::string PASS = "H9iYJQwUZIOj26awMCcqskosJ6tUXnu+LsTudw+h8uDPfmKB9lbEi89yAyMu3iowXg7aN1FpnFccb9QWx0F9utjOLs289saC1sGPl3cGrHg86HmJxGd/7YMnPox4Perl3gU66f6YzAsL1OkMmctu1/AraqYMfbkIhtzkPE61Gjg=";


    NetClient* client = nullptr;
    int _myEntityId = -1;
    int _lastTickId = -1;
    Entity* _player = nullptr;
    int _lastUpdate = -1;

    std::vector<Entity*> entities{};
    std::unordered_map<int, Entity*> entityDic{};

    arc::Stopwatch _sw;
    float acc{};

    void create() override {


        const std::vector<unsigned char> KEY1 = {0x6a,0x39,0x57,0x0c,0xc9,0xde,0x4e,0xc7,0x1d,0x64,0x82,0x18,0x94};
        const std::vector<unsigned char> KEY2 = {0xc7,0x93,0x32,0xb1,0x97,0xf9,0x2b,0xa8,0x5e,0xd2,0x81,0xa0,0x23};

        net::init();
        client = new NetClient(KEY1, KEY2);
        client->registerTypes(
                [](unsigned char packetId) -> IPacket* {

                    switch (static_cast<PacketType>(packetId)) {

                        case PacketType::FAILURE: return static_cast<IPacket*>(new FailurePacket());
                        case PacketType::MAPINFO: return static_cast<IPacket*>(new MapInfoPacket());
                        case PacketType::CREATE_SUCCESS: return static_cast<IPacket*>(new CreateSuccessPacket());
                        case PacketType::UPDATE: return static_cast<IPacket*>(new UpdatePacket());
                        case PacketType::NEWTICK: return static_cast<IPacket*>(new NewTickPacket());
                        case PacketType::TEXT: return static_cast<IPacket*>(new TextPacket());
                        case PacketType::SHOWEFFECT: return static_cast<IPacket*>(new ShowEffectPacket());
                        case PacketType::PING: return static_cast<IPacket*>(new PingPacket());
                        case PacketType::ALLYSHOOT: return static_cast<IPacket*>(new AllyShootPacket());
                        case PacketType::GLOBAL_NOTIFICATION: return static_cast<IPacket*>(new GlobalNotificationPacket());
                        case PacketType::ACCOUNTLIST: return static_cast<IPacket*>(new AccountListPacket());
                        case PacketType::NOTIFICATION: return static_cast<IPacket*>(new NotificationPacket());
                        case PacketType::GOTO: return static_cast<IPacket*>(new GotoPacket());
                        case PacketType::GOTOACK: return static_cast<IPacket*>(new GotoAckPacket());
                        case PacketType::RECONNECT: return static_cast<IPacket*>(new ReconnectPacket());
                        default:
                            return nullptr;
                    }

                }
        );
        client->connect("52.47.149.74", 2050);
    }

    void update(float dt) override {

        auto t = static_cast<int>(_sw.elapsed());
        auto dtMS = t - _lastUpdate;

        while(client->hasMessages())
        {
            auto msg = client->getNextMessage();
            switch (msg->eventType)
            {
                case EventType::Connected:
                    onConnected();
                    break;
                case EventType::Disconnected:
                    arc::log::info("Disconnected!");
                    break;
                case EventType::Packet:
                    //arc::log::infof("PACKET: {0}", (int)msg.packet->id());
                    handlePacket(static_cast<PacketType>(msg->packet->id()), msg->packet);
                    break;
                default:
                    arc::log::infof("Non handled event type: {0}", (int) msg->eventType);
                    break;
            }

            delete msg;
        }


        acc += dt;
        if(acc > 1.0)
        {
            //arc::log::infof("Entity Dic: {0}", (int) entityDic.size());
            acc = 0.0;
        }


        _lastUpdate = t;
    }

    void render(float dt) override {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        glEnable(GL_DEPTH_TEST);

    }

    void resize(int width, int height) override {

    }

    void dispose() override {

        printf("Bye!\n");
        client->disconnect();
        delete client;
    }

    void onConnected()
    {
        arc::log::info("Connected!");

        _sw.start();

        auto hello = HelloPacket();
        hello.buildVersion = BUILD;
        hello.guid = GUID;
        hello.password = PASS;
        hello.gameId = -2;
        hello.random1 = 120028554;
        hello.random1 = 838391817;
        hello.gameNet = "rotmg";
        hello.playPlatform = "rotmg";

        client->sendPacket(hello);

        //auto hello = arc::file::loadFile("UPDATE.dat");
        //client->sendPacket(hello);
    }

    void handlePacket(PacketType id, IPacket* packet)
    {
        if(id == PacketType::FAILURE)
        {
            auto failure = dynamic_cast<FailurePacket*>(packet);
            arc::log::errorf("FAILURE: {0}:{1}", failure->errorId, failure->errorDescription);
        }
        else if(id == PacketType::MAPINFO)
        {
            auto mapInfo = dynamic_cast<MapInfoPacket*>(packet);
            arc::log::infof("MAP: {0} Size: {1}:{2}", mapInfo->name, mapInfo->width, mapInfo->height);

            auto load = LoadPacket();
            load.characterId = 2;

            client->sendPacket(load);
        }
        else if(id == PacketType::CREATE_SUCCESS)
        {
            auto createSuccess = dynamic_cast<CreateSuccessPacket*>(packet);
            _myEntityId = createSuccess->objectId;
        }
        else if(id == PacketType::TEXT)
        {
            auto text = dynamic_cast<TextPacket*>(packet);
            arc::log::infof("TEXT: [{0}] ({1}) {2}", text->numStars, text->name, text->text);
        }
        else if(id == PacketType::UPDATE)
        {
            auto ack = UpdateAckPacket();
            client->sendPacket(ack);

            auto update = dynamic_cast<UpdatePacket*>(packet);
            for(const auto& tile : update->tiles)
            {

            }

            for(const auto& netEntity : update->entities)
            {
                auto entity = new Entity();
                entity->entityType = netEntity.objectType;
                entity->entityId = netEntity.status.objectId;
                entity->x = netEntity.status.position.x;
                entity->y = netEntity.status.position.y;
                entity->parseData(netEntity.status.data);

                entities.emplace_back(entity);
                entityDic[entity->entityId] = entity;

                if(entity->entityId == _myEntityId)
                {
                    _player = entity;
                }
            }

            for(const auto& drop : update->drops)
            {
                auto got = entityDic.find(drop);
                if(got != entityDic.end())
                {
                    auto* entity = got->second;
                    entities.erase(std::remove(entities.begin(), entities.end(), entity), entities.end());
                    entityDic.erase(got);
                    delete entity;
                } else
                    arc::log::errorf("Can't find entity to remove with ID: {0}", drop);
            }
        }
        else if(id == PacketType::NEWTICK)
        {
            auto newtick = dynamic_cast<NewTickPacket*>(packet);
            if(_player)
            {
                auto move = MovePacket();
                move.tickId = newtick->tickId;
                move.time = _lastUpdate;
                move.newPosition.x = _player->x;
                move.newPosition.y = _player->y;
                client->sendPacket(move);
            }
        }
        else if(id == PacketType::PING)
        {
            auto ping = dynamic_cast<PingPacket*>(packet);
            auto pong = PongPacket();
            pong.serial = ping->serial;
            pong.time =  static_cast<int>(_sw.elapsed());
            client->sendPacket(pong);
        }
        else if(id == PacketType::GOTO)
        {
            auto gt = dynamic_cast<GotoPacket*>(packet);

            auto gtAck = GotoAckPacket();
            gtAck.time = _lastUpdate;
            client->sendPacket(gtAck);

            auto got = entityDic.find(gt->objectId);
            if(got != entityDic.end()) {
                auto* entity = got->second;
                entity->onGoto(gt->location.x, gt->location.y);
            }
        }
        else if(id == PacketType::SHOWEFFECT) {}
        else if(id == PacketType::ALLYSHOOT) {}
        else if(id == PacketType::NOTIFICATION) {}
        else
        {
           arc::log::errorf("Packet: {0} not handled", (int) id);
        }
    }
};

int main(int argc, char** argv) {
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    auto config = arc::Configuration();
    config.windowTitle = "Sample 99 - Experiments";
    auto myGame = new MyGame();
    auto engine = new arc::Engine(myGame, config);
    engine->run();

    return 0;
}

void testStuff()
{
    std::vector<unsigned char> buffer;
    buffer.resize(1024 * 10);


    PWriter writer{buffer};

    auto span = arc::Span<unsigned char>{&buffer[0], (int) buffer.size()};
    PReader reader{span};


    writer.writeInt(16);
    writer.writeFloat(32.0);

    printf("%d\n", reader.readInt());
    printf("%f\n", reader.readFloat());
}