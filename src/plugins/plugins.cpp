#include "plugins.h"
#include <plugins/events/serverevents.hpp>
#include <plugins/events/clientevents.hpp>
#include <utility>
#include <utils/logger.h>
#include <plugins/event.h>
#include <net/luaregnet.hpp>
#include <utils/luaregutils.hpp>
#include <plugins/events/luaregevents.hpp>

Plugin::Plugin(std::string path) : path(std::move(path)), state(nullptr)
{
}

Plugin::~Plugin()
{
    lua_close(state);
}

bool Plugin::load()
{
    state = luaL_newstate();

    defineLibs();

    if (luaL_dofile(state, path.c_str()) != 0)
    {
        logger::error("Could not load plugin at '%s'", path.c_str());
        logger::error("%s", lua_tostring(state, -1));
        return false;
    }

    auto luaName = luabridge::getGlobal(state, "Name");
    auto luaVersion = luabridge::getGlobal(state, "Version");
    if (!luaName.isString() || !luaVersion.isString())
    {
        logger::error("Plugin has invalid metadata at %s", path.c_str());
    }
    name = luaName.tostring();
    version = luaVersion.tostring();

    return true;
}

void Plugin::defineLibs()
{
    lua::registerDefaultLibs(state, name.c_str());

    loadEventsLua(state);
    loadUtilsLua(state);
    loadNetLua(state);
}

PluginsManager::PluginsManager() : plugins()
{
}

PluginsManager::~PluginsManager()
{
}

void PluginsManager::load()
{
    plugins.clear();

    if (!std::filesystem::exists(BASE_PATH) && !std::filesystem::create_directories(BASE_PATH))
    {
        logger::error("Could not create or find plugins folder !");
        return;
    }

    for (auto &entry : std::filesystem::directory_iterator(BASE_PATH))
    {
        if (entry.path().extension() == ".lua")
        {
            auto plugin = std::make_unique<Plugin>(entry.path().string());
            if (plugin->load())
            {
                plugins.push_back(std::move(plugin));
            }
        }
    }

    logger::debug("Loaded %d plugins !", plugins.size());
}
