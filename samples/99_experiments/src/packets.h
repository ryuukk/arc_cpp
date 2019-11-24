#pragma once


class IPacket
{
public:
    ~IPacket() = default;;
    virtual void read(PReader& reader) = 0;
    virtual void write(PWriter& writer) = 0;
    virtual unsigned char id() = 0;
};

enum class StatsType : unsigned char
{
    MAX_HP_STAT = 0,
    HP_STAT = 1,
    SIZE_STAT = 2,
    MAX_MP_STAT = 3,
    MP_STAT = 4,
    NEXT_LEVEL_EXP_STAT = 5,
    EXP_STAT = 6,
    LEVEL_STAT = 7,
    ATTACK_STAT = 20,
    DEFENSE_STAT = 21,
    SPEED_STAT = 22,
    INVENTORY_0_STAT = 8,
    INVENTORY_1_STAT = 9,
    INVENTORY_2_STAT = 10,
    INVENTORY_3_STAT = 11,
    INVENTORY_4_STAT = 12,
    INVENTORY_5_STAT = 13,
    INVENTORY_6_STAT = 14,
    INVENTORY_7_STAT = 15,
    INVENTORY_8_STAT = 16,
    INVENTORY_9_STAT = 17,
    INVENTORY_10_STAT = 18,
    INVENTORY_11_STAT = 19,
    VITALITY_STAT = 26,
    WISDOM_STAT = 27,
    DEXTERITY_STAT = 28,
    CONDITION_STAT = 29,
    NUM_STARS_STAT = 30,
    NAME_STAT = 31,
    TEX1_STAT = 32,
    TEX2_STAT = 33,
    MERCHANDISE_TYPE_STAT = 34,
    CREDITS_STAT = 35,
    MERCHANDISE_PRICE_STAT = 36,
    ACTIVE_STAT = 37,
    ACCOUNT_ID_STAT = 38,
    FAME_STAT = 39,
    MERCHANDISE_CURRENCY_STAT = 40,
    CONNECT_STAT = 41,
    MERCHANDISE_COUNT_STAT = 42,
    MERCHANDISE_MINS_LEFT_STAT = 43,
    MERCHANDISE_DISCOUNT_STAT = 44,
    MERCHANDISE_RANK_REQ_STAT = 45,
    MAX_HP_BOOST_STAT = 46,
    MAX_MP_BOOST_STAT = 47,
    ATTACK_BOOST_STAT = 48,
    DEFENSE_BOOST_STAT = 49,
    SPEED_BOOST_STAT = 50,
    VITALITY_BOOST_STAT = 51,
    WISDOM_BOOST_STAT = 52,
    DEXTERITY_BOOST_STAT = 53,
    OWNER_ACCOUNT_ID_STAT = 54,
    RANK_REQUIRED_STAT = 55,
    NAME_CHOSEN_STAT = 56,
    CURR_FAME_STAT = 57,
    NEXT_CLASS_QUEST_FAME_STAT = 58,
    LEGENDARY_RANK_STAT = 59,
    SINK_LEVEL_STAT = 60,
    ALT_TEXTURE_STAT = 61,
    GUILD_NAME_STAT = 62,
    GUILD_RANK_STAT = 63,
    BREATH_STAT = 64,
    XP_BOOSTED_STAT = 65,
    XP_TIMER_STAT = 66,
    LD_TIMER_STAT = 67,
    LT_TIMER_STAT = 68,
    HEALTH_POTION_STACK_STAT = 69,
    MAGIC_POTION_STACK_STAT = 70,
    BACKPACK_0_STAT = 71,
    BACKPACK_1_STAT = 72,
    BACKPACK_2_STAT = 73,
    BACKPACK_3_STAT = 74,
    BACKPACK_4_STAT = 75,
    BACKPACK_5_STAT = 76,
    BACKPACK_6_STAT = 77,
    BACKPACK_7_STAT = 78,
    HASBACKPACK_STAT = 79,
    TEXTURE_STAT = 80,
    PET_INSTANCEID_STAT = 81,
    PET_NAME_STAT = 82,
    PET_TYPE_STAT = 83,
    PET_RARITY_STAT = 84,
    PET_MAXABILITYPOWER_STAT = 85,
    PET_FAMILY_STAT = 86,
    PET_FIRSTABILITY_POINT_STAT = 87,
    PET_SECONDABILITY_POINT_STAT = 88,
    PET_THIRDABILITY_POINT_STAT = 89,
    PET_FIRSTABILITY_POWER_STAT = 90,
    PET_SECONDABILITY_POWER_STAT = 91,
    PET_THIRDABILITY_POWER_STAT = 92,
    PET_FIRSTABILITY_TYPE_STAT = 93,
    PET_SECONDABILITY_TYPE_STAT = 94,
    PET_THIRDABILITY_TYPE_STAT = 95,
    NEW_CON_STAT = 96,
    FORTUNE_TOKEN_STAT = 97,
    SUPPORTER_POINTS_STAT = 98,
    SUPPORTER_STAT = 99,
    CHALLENGER_STARBG_STAT = 100
};

struct StatData
{
    StatData() = default;

    StatsType id;
    //union {
        int intValue;
        std::string stringValue;
    //} data;

    bool isUTF()
    {
        switch (id)
        {
            case StatsType::NAME_STAT:
            case StatsType::GUILD_NAME_STAT:
            case StatsType::PET_NAME_STAT:
            case StatsType::ACCOUNT_ID_STAT:
            case StatsType::OWNER_ACCOUNT_ID_STAT:
                return true;
            default:
                return false;
        }
    }

    void read(PReader& reader)
    {
        id = static_cast<StatsType>(reader.readByte());
        if(isUTF())
            stringValue = reader.readString();
        else
            intValue = reader.readInt();
    }
};

enum class PacketType : unsigned char
{
    UNKNOW = 255,

    FAILURE = 0,
    CREATE_SUCCESS = 101,
    CREATE = 61,
    PLAYERSHOOT = 30,
    MOVE = 42,
    PLAYERTEXT = 10,
    TEXT = 44,
    SERVERPLAYERSHOOT = 12,
    DAMAGE = 75,
    UPDATE = 62,
    UPDATEACK = 81,
    NOTIFICATION = 67,
    NEWTICK = 9,
    INVSWAP = 19,
    USEITEM = 11,
    SHOWEFFECT = 13,
    HELLO = 1,
    GOTO = 18,
    INVDROP = 55,
    INVRESULT = 95,
    RECONNECT = 45,
    PING = 8,
    PONG = 31,
    MAPINFO = 92,
    LOAD = 57,
    PIC = 83,
    SETCONDITION = 60,
    TELEPORT = 74,
    USEPORTAL = 47,
    DEATH = 46,
    BUY = 85,
    BUYRESULT = 22,
    AOE = 64,
    GROUNDDAMAGE = 103,
    PLAYERHIT = 90,
    ENEMYHIT = 25,
    AOEACK = 89,
    SHOOTACK = 100,
    OTHERHIT = 20,
    SQUAREHIT = 40,
    GOTOACK = 65,
    EDITACCOUNTLIST = 27,
    ACCOUNTLIST = 99,
    QUESTOBJID = 82,
    CHOOSENAME = 97,
    NAMERESULT = 21,
    CREATEGUILD = 59,
    GUILDRESULT = 26,
    GUILDREMOVE = 15,
    GUILDINVITE = 104,
    ALLYSHOOT = 49,
    ENEMYSHOOT = 35,
    REQUESTTRADE = 5,
    TRADEREQUESTED = 88,
    TRADESTART = 86,
    CHANGETRADE = 56,
    TRADECHANGED = 28,
    ACCEPTTRADE = 36,
    CANCELTRADE = 91,
    TRADEDONE = 34,
    TRADEACCEPTED = 14,
    CLIENTSTAT = 69,
    CHECKCREDITS = 102,
    ESCAPE = 105,
    FILE = 106,
    INVITEDTOGUILD = 77,
    JOINGUILD = 7,
    CHANGEGUILDRANK = 37,
    PLAYSOUND = 38,
    GLOBAL_NOTIFICATION = 66,
    RESKIN = 51,
    PETUPGRADEREQUEST = 16,
    ACTIVE_PET_UPDATE_REQUEST = 24,
    ACTIVEPETUPDATE = 76,
    NEW_ABILITY = 41,
    PETYARDUPDATE = 78,
    EVOLVE_PET = 87,
    DELETE_PET = 4,
    HATCH_PET = 23,
    ENTER_ARENA = 17,
    IMMINENT_ARENA_WAVE = 50,
    ARENA_DEATH = 68,
    ACCEPT_ARENA_DEATH = 80,
    VERIFY_EMAIL = 39,
    RESKIN_UNLOCK = 107,
    PASSWORD_PROMPT = 79,
    QUEST_FETCH_ASK = 98,
    QUEST_REDEEM = 58,
    QUEST_FETCH_RESPONSE = 6,
    QUEST_REDEEM_RESPONSE = 96,
    PET_CHANGE_FORM_MSG = 53,
    KEY_INFO_REQUEST = 94,
    KEY_INFO_RESPONSE = 63,
    CLAIM_LOGIN_REWARD_MSG = 3,
    LOGIN_REWARD_MSG = 93,
    QUEST_ROOM_MSG = 48,
    PET_CHANGE_SKIN_MSG = 33,
    REALM_HERO_LEFT_MSG = 84,
    RESET_DAILY_QUESTS = 52
};

class UnknowPacket : public IPacket
{

public:
    void read(PReader& reader) override {

    }

    void write(PWriter& writer) override {

    }

    unsigned char id() override {
        return static_cast<unsigned char>(PacketType::UNKNOW);
    }

};
class ReconnectPacket : public IPacket
{

public:
    void read(PReader& reader) override {

    }

    void write(PWriter& writer) override {

    }

    unsigned char id() override {
        return static_cast<unsigned char>(PacketType::RECONNECT);
    }

};
class NotificationPacket : public IPacket
{

public:
    void read(PReader& reader) override {

    }

    void write(PWriter& writer) override {

    }

    unsigned char id() override {
        return static_cast<unsigned char>(PacketType::NOTIFICATION);
    }

};
class AccountListPacket : public IPacket
{

public:
    void read(PReader& reader) override {

    }

    void write(PWriter& writer) override {

    }

    unsigned char id() override {
        return static_cast<unsigned char>(PacketType::ACCOUNTLIST);
    }

};
class GlobalNotificationPacket : public IPacket
{

public:
    void read(PReader& reader) override {

    }

    void write(PWriter& writer) override {

    }

    unsigned char id() override {
        return static_cast<unsigned char>(PacketType::GLOBAL_NOTIFICATION);
    }

};
class ShowEffectPacket : public IPacket
{

public:
    void read(PReader& reader) override {

    }

    void write(PWriter& writer) override {

    }

    unsigned char id() override {
        return static_cast<unsigned char>(PacketType::SHOWEFFECT);
    }

};
class AllyShootPacket : public IPacket
{

public:
    void read(PReader& reader) override {

    }

    void write(PWriter& writer) override {

    }

    unsigned char id() override {
        return static_cast<unsigned char>(PacketType::ALLYSHOOT);
    }

};

class FailurePacket : public IPacket
{
public:
    int errorId;
    std::string errorDescription;
public:
    void read(PReader& reader) override {
        errorId = reader.readInt();
        errorDescription = reader.readString();
    }

    void write(PWriter& writer) override {
        writer.writeInt(errorId);
        writer.writeString(errorDescription);
    }

    unsigned char id() override {
        return static_cast<unsigned char>(PacketType::FAILURE);
    }

};

class MapInfoPacket : public IPacket
{

public:
    int width{};
    int height{};
    std::string name{};
    std::string displayName{};
    int difficulty{};
    unsigned int fp{};
    int background{};
    bool allowPlayerTeleport{};
    bool showDisplays{};
    std::vector<std::string> clientXml{};
    std::vector<std::string> extraXml{};

public:
    void read(PReader& reader) override {
        width = reader.readInt();
        height = reader.readInt();
        name = reader.readString();
        displayName = reader.readString();
        difficulty = reader.readInt();
        fp = reader.readUInt();
        background = reader.readInt();
        allowPlayerTeleport = reader.readBool();
        showDisplays = reader.readBool();


        auto cx = reader.readShort();
        clientXml.resize(cx);
        for (int i = 0; i < cx; i++)
            clientXml[i] = reader.readUTF32();

        auto ex = reader.readShort();
        extraXml.resize(ex);
        for (int i = 0; i < ex; i++)
            extraXml[i] = reader.readUTF32();

    }

    void write(PWriter& writer) override {

    }

    unsigned char id() override {
        return static_cast<unsigned char>(PacketType::MAPINFO);
    }

};

class LoadPacket : public IPacket
{
public:
    int characterId;
    bool isFromArena;
    bool isSupporter;
public:
    void read(PReader& reader) override {

        characterId = reader.readInt();
        isFromArena = reader.readBool();
        isSupporter = reader.readBool();
    }

    void write(PWriter& writer) override {

        writer.writeInt(characterId);
        writer.writeBool(isFromArena);
        writer.writeBool(isSupporter);
    }

    unsigned char id() override {
        return static_cast<unsigned char>(PacketType::LOAD);
    }

};
class CreateSuccessPacket : public IPacket
{
public:
    int objectId;
    int charId;

public:
    void read(PReader& reader) override {

        objectId = reader.readInt();
        charId = reader.readInt();
    }

    void write(PWriter& writer) override {

        writer.writeInt(objectId);
        writer.writeInt(charId);
    }

    unsigned char id() override {
        return static_cast<unsigned char>(PacketType::CREATE_SUCCESS);
    }
};
class PingPacket : public IPacket
{
public:
    int serial;
public:
    void read(PReader& reader) override {

        serial = reader.readInt();
    }

    void write(PWriter& writer) override {
        writer.writeInt(serial);
    }

    unsigned char id() override {
        return static_cast<unsigned char>(PacketType::PING);
    }
};
class PongPacket : public IPacket
{
public:
    int serial;
    int time;

public:
    void read(PReader& reader) override {
        serial = reader.readInt();
        time = reader.readInt();
    }

    void write(PWriter& writer) override {
        writer.writeInt(serial);
        writer.writeInt(time);
    }

    unsigned char id() override {
        return static_cast<unsigned char>(PacketType::PONG);
    }
};

struct Tile
{
    short x;
    short y;
    unsigned  type;
};

struct Status
{
    int objectId;
    arc::Vec2 position;
    std::vector<StatData> data;

    void read(PReader& reader)
    {
        objectId = reader.readInt();

        position.x = reader.readFloat();
        position.y = reader.readFloat();
        auto s = reader.readShort();
        data.resize(s);
        for (int i = 0; i < s; i++)
        {
            StatData stat;
            stat.read(reader);
            data[i] = stat;
        }
    }
};

struct NetEntity
{
    int objectType;
    Status status;
    void read(PReader& reader)
    {
        objectType = reader.readUShort();
        status.read(reader);
    }
};

class UpdatePacket : public IPacket
{
public:
    std::vector<Tile> tiles;
    std::vector<NetEntity> entities;
    std::vector<int> drops;
public:
    void read(PReader& reader) override {
        auto t = reader.readShort();
        tiles.resize(t);
        for (int i = 0; i < t; i++)
        {
            auto tile = Tile();
            tile.x = reader.readShort();
            tile.y = reader.readShort();

            tile.type = reader.readUShort();
            tiles[i] = tile;
        }

        auto e = reader.readShort();

        entities.resize(e);
        for (int i = 0; i < e; i++)
        {
            auto entity = NetEntity();
            entity.read(reader);
            entities[i] = entity;
        }

        auto d = reader.readShort();
        drops.resize(d);
        for (int i = 0; i < d; i++)
        {
            drops[i] = reader.readInt();
        }
    }

    void write(PWriter& writer) override {

    }

    unsigned char id() override {
        return static_cast<unsigned char>(PacketType::UPDATE);
    }
};
class UpdateAckPacket : public IPacket
{

public:
    void read(PReader& reader) override {

    }

    void write(PWriter& writer) override {

    }

    unsigned char id() override {
        return static_cast<unsigned char>(PacketType::UPDATEACK);
    }
};

class NewTickPacket : public IPacket
{
public:
    int tickId;
    int tickTime;
    std::vector<Status> statuses;
public:
    void read(PReader& reader) override {
        tickId = reader.readInt();
        tickTime = reader.readInt();

        auto s = reader.readShort();
        statuses.resize(s);
        for (int i = 0; i < s; i++)
        {
            auto status = Status();
            status.read(reader);
            statuses[i] = status;
        }
    }

    void write(PWriter& writer) override {

    }

    unsigned char id() override {
        return static_cast<unsigned char>(PacketType::NEWTICK);
    }
};


struct MoveRecord
{
    int time;
    arc::Vec2 pos;
};
class MovePacket : public IPacket
{
public:
    int tickId;
    int time;
    arc::Vec2 newPosition;
    std::vector<MoveRecord> records;
public:
    void read(PReader& reader) override {

    }

    void write(PWriter& writer) override {
        writer.writeInt(tickId);
        writer.writeInt(time);
        writer.writeFloat(newPosition.x);
        writer.writeFloat(newPosition.y);
        writer.writeShort((short) records.size());
        for (const auto & rec : records)
        {
            writer.writeInt(rec.time);
            writer.writeFloat(rec.pos.x);
            writer.writeFloat(rec.pos.y);
        }
    }

    unsigned char id() override {
        return static_cast<unsigned char>(PacketType::MOVE);
    }
};
class GotoPacket : public IPacket
{
public:
    int objectId;
    arc::Vec2 location;
public:
    void read(PReader& reader) override {
        objectId = reader.readInt();
        location.x = reader.readFloat();
        location.y = reader.readFloat();
    }

    void write(PWriter& writer) override {

    }

    unsigned char id() override {
        return static_cast<unsigned char>(PacketType::GOTO);
    }
};
class GotoAckPacket : public IPacket
{
public:
    int time;
public:
    void read(PReader& reader) override {
        time = reader.readInt();
    }

    void write(PWriter& writer) override {
        writer.writeInt(time);
    }

    unsigned char id() override {
        return static_cast<unsigned char>(PacketType::GOTOACK);
    }
};
class TextPacket : public IPacket
{
public:
    std::string name;
    int objectId;
    int numStars;
    unsigned char bubbleTime;
    std::string recipient;
    std::string text;
    std::string cleanText;
    bool isSupporter;
public:
    void read(PReader& reader) override {
        name = reader.readString();
        objectId = reader.readInt();
        numStars = reader.readInt();
        bubbleTime = reader.readByte();
        recipient = reader.readString();
        text = reader.readString();
        cleanText = reader.readString();
        isSupporter = reader.readBool();
    }

    void write(PWriter& writer) override {

    }

    unsigned char id() override {
        return static_cast<unsigned char>(PacketType::TEXT);
    }
};

class HelloPacket : public IPacket
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
    std::string key;
    std::string mapJson;
    std::string entryTag;
    std::string gameNet;
    std::string gameNetUserId;
    std::string playPlatform;
    std::string platformToken;
    std::string userToken;

public:
    void read(PReader& reader) override {
        buildVersion = reader.readString();
        gameId = reader.readInt();
        guid = reader.readString();
        random1 = reader.readInt();
        password = reader.readString();
        random2 = reader.readInt();
        secret = reader.readString();
        keyTime = reader.readInt();
        key = reader.readString();
        mapJson = reader.readUTF32();
        entryTag = reader.readString();
        gameNet = reader.readString();
        gameNetUserId = reader.readString();
        playPlatform = reader.readString();
        platformToken = reader.readString();
        userToken = reader.readString();

        reader.readString();

    }

    void write(PWriter& writer) override {
        writer.writeString(buildVersion);
        writer.writeInt(gameId);
        writer.writeString(guid);
        writer.writeInt(random1);
        writer.writeString(password);
        writer.writeInt(random2);
        writer.writeString(secret);
        writer.writeInt(keyTime);
        writer.writeString(key);
        writer.writeUTF32(mapJson);
        writer.writeString(entryTag);
        writer.writeString(gameNet);
        writer.writeString(gameNetUserId);
        writer.writeString(playPlatform);
        writer.writeString(platformToken);
        writer.writeString(userToken);
        writer.writeString("XTeP7hERdchV5jrBZEYNebAqDPU6tKU6");
    }

    unsigned char id() override {
        return static_cast<unsigned char>(PacketType::HELLO);
    }
};