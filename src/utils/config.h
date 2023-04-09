/**
 * @file config.h
 * @author Mathieu Cayeux
 * @brief The main file for the config io
 * @version 1.0
 * @date 2023-03-20
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef MINESERVER_CONFIG_H
#define MINESERVER_CONFIG_H

#include <memory>
#include <rapidjson/document.h>

/**
 * @brief The Field Object for the ::Config
 *
 * The field object should only be used internally,
 * as it is only used in ::Config for parsing / writing
 * values to the config file.
 * @tparam T the value type of the field
 */
template <typename T>
class Field
{
private:
    const char *section;
    const char *key;
    T value;

    inline rapidjson::Document::ConstMemberIterator canSafelyRead(const rapidjson::Document &document);
    inline void writeSafely(rapidjson::Document &document, rapidjson::Value &v);

public:
    /**
     * @brief Construct a new Field object
     *
     * Should only be used internally, constructs a new object
     * that will be used in the ::Config for parsing / writing
     * values to the config file.
     * @param section The section of the field
     * @param key The key / name of the field
     * @param def Its default value
     */
    Field(const char *section, const char *key, T def);
    /**
     * @brief Destroy the Field object
     */
    ~Field();

    /**
     * @brief Loads the value from the config
     *
     * Loads from the json document the value,
     * parsing it.
     * @param document The Json document
     * @todo Contains an ugly of copying the string
     */
    void load(const rapidjson::Document &document);
    /**
     * @brief Saves value to the config
     *
     * Saves value to the json document.
     * @param document The Json document
     */
    void save(rapidjson::Document &document);

    /**
     * @brief Get the Value of the field
     *
     * @return const T& the type of the field
     */
    const T &getValue()
    {
        return value;
    }

    /**
     * @brief Set the Value of the field
     *
     * @param v the new value
     */
    void setValue(T v)
    {
        value = v;
    }
};

/**
 * @brief The config class
 *
 * The (uselessly big) class that take care
 * of the loading, saving of the configuration
 * values from the config file.
 * The config is a singleton.
 */
class Config
{
private:
    static Config *INSTANCE;

public:
    /**
     * @brief Construct a new Config object
     *
     * Should only be used once, in main.
     * Creating other objects will be useless as
     * they will not be initialized.
     */
    Config();
    /**
     * @brief Destroy the Config object
     *
     */
    ~Config();

    /**
     * @brief Saves the config on disk
     *
     * Writes the content of the fields on disk, saving
     * all of it !
     */
    void save();
    /**
     * @brief Loads the config from disk
     *
     * Loads the config by reading the file on
     * disk as a Json Object and loading them
     * into the fields.
     */
    void load();

    /**
     * @brief The port of the instance
     *
     * The port on which the server should
     * be listening on.
     */
    Field<int> PORT = Field("network", "port", 25565);
    /**
     * @brief The compression level for ZLib
     *
     * The compression level to use with ZLib when
     * compressing packets.
     */
    Field<int> COMPRESSION_LVL = Field("network", "compression_level", -1);
    /**
     * @brief The online mode flag
     *
     * Wether to check with mojang when an account
     * is logging in if the said account is crack
     * or not.
     */
    Field<bool> ONLINE_MODE = Field("network", "online_mode", true);
    /**
     * @brief The Message of the Day
     *
     * The message of the day to be displayed
     * on a status update.
     */
    Field<std::string> MOTD = Field("display", "motd", std::string("This is the message of the day !"));
    /**
     * @brief The Log Level
     *
     * The minimum ::LogLevel that the ::logger
     * should use.
     */
    Field<std::string> LOGLEVEL = Field("other", "loglevel", std::string("ALL"));

/**
 * @brief List of all the config fields
 *
 * List of all of the above config fields, used
 * as a handy tool to run similar functions
 * on all of the fields by defining the UF(x) macro.
 */
#define CONFIG_FIELDS UF(PORT) UF(MOTD) UF(LOGLEVEL) UF(COMPRESSION_LVL) UF(ONLINE_MODE)

    /**
     * @brief Fetch the instance of the config
     *
     * Gets the instance of the config, following
     * the singleton class-like.
     * @return ::Config* The instance of the config
     */
    static Config *inst()
    {
        return INSTANCE;
    }
};

#endif // MINESERVER_CONFIG_H
