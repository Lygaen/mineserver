# Plugins
[TOC]

You can make custom plugins for your server in [Lua](https://www.lua.org/about.html).
We chose Lua because it is fast, lightweight, easy to learn and more importantly easy to use on top of C++.

## Basics
First of all, you need to name your plugin and give it a version.
The filename of your file doesn't matter, the name that will be displayed
inside the game will be retrieved from the Lua file.
You can access the current plugin running metadata as such :

```lua
-- The following two lines should be at the top of your file
plugin.name = "Random plugin name"
plugin.version = "1.0.0"

-- Here it is just for show sake
print("Plugin path is " .. plugin.path)
```
## Events
You can listen to all the events listed in the [events directory](@ref src/plugins/events).
Go click on the individual files, look for the events you want to listen and you
should be able to see in their documentation how to listen for the event.
Generally to listen for an event a class called `SimpleBasicEvent`, you do
as follows :
```lua
local function canBeAnything(e)
    -- do your thing
end

-- register the function as a listener
event.onSimpleBasic(canBeAnything)
```


## Access to classes
You can normally access all of the classes of the server.
It is still at a development stage so not everything is well connected
and not everything will have a lua equivalent.
There are still few things you can do though.

### Generally
If it is not specified in the class documentation, you can
access a class fields like they are written.
For exemple, for the [Handshake Packet class](@ref HandshakePacket), you
can access its [Next State field](@ref HandshakePacket::nextState) field
like this, after for exemple registering for the [Client Handshake Event](@ref ClientHandshakeEvent) :
```lua
local function onStatus(e)
    -- do your thing, like printing the client protocol version
    print("Client protocol version : " .. e.packet.protocolVersion)
end

event.onClientHandshake(onStatus)
```
### Config
You can access any fields of the config using the following manner.
If you want to access [the max players field](@ref Config::MAX_PLAYERS) for exemple,
refer to the [Config Documentation](_2_CONFIG.md) and get the category of the field
and its name so that if you want to access it, you do :
```lua
config.&lt;category&gt;.&lt;fieldname&gt;
```
For exemple, for the max players field in the `Server` category :
```lua
-- Accessing field
config.server.max_players

print("Max number of players : " .. config.server.max_players)

-- Lets add 10
config.server.max_players = config.server.max_players + 10
```

## Libraries Warning
All default libraries are opened on the lua file. So beware of what files you install because they can use
libraries like `os` that can modify files on your computer.