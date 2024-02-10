/**
 * @file plugins.h
 * @author Lygaen
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

    void defineLibs();

public:
    /**
     * @brief Name of the plugin
     *
     */
    std::string name;
    /**
     * @brief Version of the plugin
     *
     */
    std::string version;
    /**
     * @brief Construct a new Plugin object
     *
     * @param path the path of the lua plugin
     */
    explicit Plugin(std::string path);
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
    std::vector<std::shared_ptr<Plugin>> plugins;

    static PluginsManager *instance;

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

    /**
     * @brief Get the registered plugins
     *
     * @return const std::vector<std::shared_ptr<Plugin>>& plugins list
     */
    const std::vector<std::shared_ptr<Plugin>> &getPlugins() const
    {
        return plugins;
    }

    /**
     * @brief Gets the instance of the plugin manager
     *
     * @return PluginsManager& the instance
     */
    static PluginsManager &inst()
    {
        return *instance;
    }
};

#endif // MINESERVER_PLUGINS_H