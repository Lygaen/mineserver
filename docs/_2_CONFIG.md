# Config
[TOC]

Mineserver uses multiple steps of configuration to customize everything. These steps are going for most influential to least. It means that for example configuration done in CMake will take over configuration done in the config file. We do not currently support configuration over the command line with arguments.
The order of hierarchy of the configuration is :
 - [CMake Definitions](#cmake_definitions) (limited and defined at compile-time)
 - Command Line Arguments (yet to be supported)
 - [config.json file](#config_file)

## CMake Definitions {#cmake_definitions}
Refer to [this piece of documentation](https://cmake.org/cmake/help/latest/prop_cache/TYPE.html) for more information on CMake types.

| Option                 | Type | Default Value | Description                                             |
|------------------------|:----:|:-------------:|---------------------------------------------------------|
| MINESERVER_ANSI_COLORS | BOOL |     TRUE      | Whether to print in the console using colors or not     |
| MINESERVER_BUILD_TESTS |  ^   |       ^       | Whether to build or not the tests                       |
| GITHUB_ACTIONS_BUILD   |  ^   |     FALSE     | Whether we are building from a Github Action (dev only) |

## Config file {#config_file}
The config is loaded at runtime from the `config.json` file.
The documentation is not to be trusted on the latest or older configurations as it is updated periodically. Go look at the [Config class](@ref Config) for an exhaustive list.
Here are the current fields listed, separated in different categories such that for example, `network` and `display` categories translate to in the config file :
```json
{
    "network": {
        // -- network members
        "port": 25565,
        "backlog": 10,
    },
    "display": {
        // -- display members
        "motd": {
            "text": "This is the message of the day !"
        },
        "icon_file": "icon.png"
    }
}
```

### Network
| Key               |  Type  | Default Value | Description                                                                  |
|-------------------|:------:|:-------------:|------------------------------------------------------------------------------|
| port              |  int   |     25565     | The port for the server to listen on                                         |
| backlog           |   ^    |      10       | The number of pending connections for the server to hold before accepting    |
| compression_level |   ^    |      -1       | The compression level for ZLib, 0 means none, 9 means best, -1 means default |
| online_mode       |  bool  |     true      | Whether to check if the client is crack or not                               |
| address           | string |   127.0.0.1   | The IP address for the server to listen on                                   |

### Display
| Key       |   Type    | Default Value | Description                                                                           |
|-----------|:---------:|:-------------:|---------------------------------------------------------------------------------------|
| motd      |   chat    |       /       | A message of the day in the form of a [Chat object](https://wiki.vg/Chat#Inheritance) |
| icon_file | file path |  ./icon.png   | Path to the png file of the server's icon (must be 64x64)                             |

### Server {#config_server_category}
| Key         | Type | Default Value | Description                   |
|-------------|:----:|:-------------:|-------------------------------|
| max_players | int  |      100      | Max number of players allowed |

### Other
| Key      |   Type   | Default Value | Description                                                               |
|----------|:--------:|:-------------:|---------------------------------------------------------------------------|
| loglevel | loglevel |      ALL      | Loglevel for the logger (ALL < DEBUG < INFO < WARN < ERROR < FATAL < OFF) |
