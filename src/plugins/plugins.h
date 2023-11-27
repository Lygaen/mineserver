/**
 * @file plugins.h
 * @author Mathieu Cayeux
 * @brief The file handling plugin system
 * @version 0.1
 * @date 2023-11-27
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef MINESERVER_PLUGINS_H
#define MINESERVER_PLUGINS_H

#include <string>
#include <filesystem>
#include <vector>
#include <plugins/luaheaders.h>

/**
 * @brief Plugin class
 *
 */
class Plugin
{
private:
    std::string path;
    lua_State *state;
    std::string name;
    std::string version;

    void defineLibs();

public:
    /**
     * @brief Construct a new Plugin object
     *
     * @param path the path of the lua plugin
     */
    Plugin(const std::string &path);
    /**
     * @brief Destroy the Plugin object
     *
     * Unloads it gracefully as well
     */
    ~Plugin();

    /**
     * @brief Loads the plugin
     *
     * @return true plugin has succeeded in loading
     * @return false plugin has failed in loading
     */
    bool load();
};

/**
 * @brief Plugin Manager
 *
 * Pretty transparent huh ?
 */
class PluginsManager
{
private:
    static constexpr std::string_view BASE_PATH = "./plugins/";
    std::vector<std::unique_ptr<Plugin>> plugins;

public:
    /**
     * @brief Construct a new Plugins Manager object
     *
     */
    PluginsManager();
    /**
     * @brief Destroy the Plugins Manager object
     *
     */
    ~PluginsManager();

    /**
     * @brief Load all plugins in plugin folder
     *
     * Can be used to reload plugins as well
     */
    void load();
};

#endif // MINESERVER_PLUGINS_H