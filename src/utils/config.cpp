#include "config.h"
#include <iostream>
#include <filesystem>

std::unique_ptr<Config> Config::instance;

bool Config::loadFromFile()
{
    bool success = file.read(data);

    if (!success)
    {
        return false;
    }

    port.parseFromMap(data);
    motd.parseFromMap(data);
    maxPlayers.parseFromMap(data);

    return true;
}

bool Config::save()
{
    port.saveToMap(data);
    motd.saveToMap(data);
    maxPlayers.saveToMap(data);

    return file.write(data, false);
}

Config::Config(int argc, char **argv)
{
    if (!std::filesystem::exists("./config.ini"))
    {
        std::ofstream ofs("./config.ini");
        ofs.close();
    }

    bool success = loadFromFile();

    if (!success)
    {
        exit(1);
    }

    std::vector<std::string> args{argv + 1, argv + argc};

    port.parseFromArgs(args);
    motd.parseFromArgs(args);
    maxPlayers.parseFromArgs(args);

    success = save();

    if (!success)
    {
        exit(1);
    }

    instance = std::unique_ptr<Config>(this);
}
