#ifndef MINESERVER_CONFIG_H
#define MINESERVER_CONFIG_H

#include <memory>
#include <rapidjson/document.h>

template<typename T>
class Field {
private:
    const char* section;
    const char* key;
    T value;

    inline rapidjson::Document::ConstMemberIterator canSafelyRead(const rapidjson::Document &document);
    inline void writeSafely(rapidjson::Document &document, rapidjson::Value& v);
public:
    Field(const char* section, const char *key, T def);
    ~Field();

    void load(const rapidjson::Document& document);
    void save(rapidjson::Document& document);

    const T &getValue() {
        return value;
    }

    T &setValue(T v) {
        value = v;
    }
};

class Config {
private:
    static Config *INSTANCE;
public:
    Config();
    ~Config();

    void save();
    void load();

    Field<int> PORT = Field("network", "port", 25565);
    Field<std::string> MOTD = Field("display", "motd", std::string("This is the message of the day !"));
    Field<std::string> LOGLEVEL = Field("other", "loglevel", std::string("ALL"));

#define CONFIG_FIELDS UF(PORT) UF(MOTD) UF(LOGLEVEL)

    static Config* inst() {
        return INSTANCE;
    }
};


#endif //MINESERVER_CONFIG_H
