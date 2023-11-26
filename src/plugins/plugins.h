#ifndef MINESERVER_PLUGINS_H
#define MINESERVER_PLUGINS_H

#include <string>
#include <filesystem>
#include <plugins/luaaa.hpp>

class Plugin
{
private:
    std::string path;
    lua_State *state;

public:
    Plugin(const std::string &path);
    ~Plugin();

    bool load();
};

class PluginSystem
{
private:
    static constexpr std::string_view BASE_PATH = "./plugins/";
    std::vector<std::unique_ptr<Plugin>> plugins;

public:
    PluginSystem();
    ~PluginSystem();

    void load();
};

#endif // MINESERVER_PLUGINS_H