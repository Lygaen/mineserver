#ifndef __MINESERVER_CONFIG_H__
#define __MINESERVER_CONFIG_H__

#include <unordered_map>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <ini.h>

#define MINESERVER_VERSION_NAME "1.8.9 MineServer"
#define MINESERVER_PROTOCOL_VERSION 47

template <typename T>
std::enable_if_t<std::is_arithmetic<T>::value, std::string> stringify(T t)
{
    return std::to_string(t);
}

template <typename T>
std::enable_if_t<!std::is_arithmetic<T>::value, std::string> stringify(T t)
{
    return static_cast<std::ostringstream &>(std::ostringstream() << t).str();
}

inline std::string stringify(std::string t)
{
    return t;
}

template <typename T>
T getStringAs(std::string &s)
{
    std::stringstream convert(s);

    T value;
    convert >> value;
    return value;
}

template <>
inline std::string getStringAs(std::string &s)
{
    return s;
}

template <typename T>
class Field
{
private:
    T value;
    const char *fieldName;
    const char *section;

public:
    Field(const char *section, const char *fieldName, T defaultValue) : section(section), value(defaultValue),
                                                                        fieldName(fieldName) {}

    void parseFromMap(const mINI::INIStructure &structure)
    {
        if (structure.has(section) && structure.get(section).has(fieldName))
        {
            auto val = structure.get(section).get(fieldName);
            value = getStringAs<T>(val);
        }
    }

    void parseFromArgs(std::vector<std::string> &args)
    {
        auto loc = std::find(args.begin(), args.end(), "--" + std::string(fieldName));
        if (loc != args.end() && std::next(loc) != args.end())
        {
            auto index = std::distance(args.begin(), std::next(loc));

            auto val = args[index];

            value = getStringAs<T>(val);
        }
    }

    void saveToMap(mINI::INIStructure &structure)
    {
        if (!structure.has(section) || !structure.get(section).has(fieldName))
        {
            structure[section][fieldName] = stringify(value);
        }
    }

    void set(T v) { value = v; }

    [[nodiscard]] T get() const { return value; }
};

class Config
{
private:
    mINI::INIFile file = mINI::INIFile("config.ini");
    mINI::INIStructure data;

    static std::unique_ptr<Config> instance;

public:
    Config(int argc, char *argv[]);

    bool loadFromFile();

    bool save();

    static std::unique_ptr<Config> &getInstance()
    {
        return instance;
    }

    Field<unsigned short> port = Field<unsigned short>("network", "port", 25565);
    Field<unsigned int> maxPlayers = Field<unsigned int>("other", "max-players", 100);
    Field<std::string> motd = Field<std::string>("display", "motd", "A MineServer !");
};

#endif // __MINESERVER_CONFIG_H__