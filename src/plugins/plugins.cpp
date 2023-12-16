/**
 * @file plugins.cpp
 * @author Lygaen
 * @brief The file containing the plugin system logic
 * @version 0.1
 * @date 2023-12-16
 *
 * @copyright Copyright (c) 2023
 *
 */

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

    return true;
}

void Plugin::defineLibs()
{
    name = path;
    version = "0";

    lua::registerDefaultLibs(state, name.c_str());

    luabridge::getGlobalNamespace(state)
        .beginNamespace("plugins")
        .beginClass<Plugin>("Plugin")
        .addProperty("name", &Plugin::name)
        .addProperty("version", &Plugin::version)
        .addProperty("path", &Plugin::path, false)
        .endClass()
        .endNamespace();
    luabridge::setGlobal(state, this, "plugin");

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
