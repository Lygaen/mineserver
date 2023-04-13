<img align="left" width="80" height="80" src="./.data/icon.png" alt="Resume application project app icon">
<h1>Mineserver</h1>

[![🧪](https://github.com/Lygaen/mineserver/actions/workflows/cmake.yml/badge.svg)](https://github.com/Lygaen/mineserver/actions/workflows/cmake.yml)
[![📚](https://github.com/Lygaen/mineserver/actions/workflows/doxygen.yml/badge.svg)](https://github.com/Lygaen/mineserver/actions/workflows/doxygen.yml)

## Summary
- [Summary](#summary)
- [Installation](#installation)
  - [Step 1 : CMake Installation](#step-1--cmake-installation)
  - [Step 2 : Make/GCC Installation](#step-2--makegcc-installation)
    - [Windows](#windows)
    - [Linux](#linux)
  - [Step 3 : Cloning](#step-3--cloning)
  - [Step 4 : Configuring](#step-4--configuring)
    - [GUI / Windows](#gui--windows)
    - [Command Line / Linux](#command-line--linux)
  - [Step 5 : Building](#step-5--building)
    - [Windows](#windows-1)
    - [Linux](#linux-1)
- [Configuration](#configuration)
  - [CMake](#cmake)
  - [Config.json](#configjson)
    - [Network](#network)
    - [Display](#display)
    - [Other](#other)
- [Contribution](#contribution)
  - [Documentation](#documentation)
  - [Code](#code)
- [Libraries](#libraries)
- [Licensing](#licensing)

## Installation
### Step 1 : CMake Installation
Minserver tries to have as few steps as possible for installation.
You don't need to install anything other than CMake on your local machine. Install it from the [website](https://cmake.org/) (on windows or other) or from your favorite package manager.

### Step 2 : Make/GCC Installation
#### Windows
This is a bit harder than linux, so it is recommended to use WSL and follow the Linux's guide. If you still want to take the hard way, here are few steps to get you going :
 - Download [MinGW from winlibs](https://winlibs.com/)
 - Unzip the downloaded file into a folder (Like `C:\MinGW`)
 - [Add the folder `<Install Path>\bin\` to your path](https://linuxhint.com/add-directory-to-path-environment-variables-windows/) (so if you installed in `C:\MinGW`, you should add `C:\MinGW\bin`)

Now you should be ready to go onto the next step.

#### Linux
You can install using your favorite package manager gcc, g++ and make.
On some distros, all the necessary tools are already installed. Please refer to your distro's documentation for the installation. When finished, you can check versions with :
```sh
make -v
gcc -v
g++ -v
```
None of the above should fail.

### Step 3 : Cloning
Open a terminal and navigate to a folder.
You can then clone this repository and its submodules using [git](https://git-scm.com/downloads) :
```sh
git clone --recurse-submodules https://github.com/Lygaen/mineserver/
```
There should be no problem if you are cloning on the default
master branch. If a problem happens or you don't have / don't want to have git on your system, try to use [releases](https://github.com/Lygaen/mineserver/releases) or the clone button on github instead.

### Step 4 : Configuring
#### GUI / Windows
If you installed CMake as described on [step 1](#step-1--cmake-installation), you should have CMake GUI (`cmake-gui` in the terminal) installed on your system.

We will be configuring it :
 - Select the cloned / downloaded source code at the top
 - Select where to build the executable at
 - Now a lot of red variables should appear in a table
 - Configure the variable starting with `MINESERVER` (refer to [this](#cmake))
 - Press `Configure` then `Generate`

#### Command Line / Linux
Now that you have the source code for mineserver, you can build it using your previously-installed cmake :
```sh
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
```
You can add `-D<YOUR OPTION>=<VALUE>` to the cmake command before the `..` to modify build parameters for Mineserver (refer to [this](#cmake)).

### Step 5 : Building
Open a terminal in the build folder or reuse the terminal in the previous steps, running the following commands. It should start building the executable. If any errors happened, please check that you did all of the previous steps or make an issue on Github, we will be happy to help !
#### Windows
```
mingw32-make
```
#### Linux
```
make
```

## Configuration
Minserver uses multiple steps of configuration to customize everything. These steps are going for most influential to least. It means that for exemple configuration done in CMake will take over configuration done in the config file. We do not currently support configuration over the command line with arguments.

### CMake
Refer to [this piece of documentation](https://cmake.org/cmake/help/latest/prop_cache/TYPE.html) for more information on CMake types.

| Option                 | Type  | Default Value | Description                                 |
| ---------------------- | :---: | :-----------: | ------------------------------------------- |
| MINESERVER_ANSI_COLORS | BOOL  |     TRUE      | Enables printing with color in the terminal |
| MINESERVER_BUILD_TESTS | BOOL  |     TRUE      | Whether to build or not the tests           |

### Config.json
Here are the different keys listed by section.
You can go take a look at [this documentation page](https://lygaen.github.io/mineserver/classConfig.html) if the description is not detailed enough (might have to scroll a bit).

#### Network
| Key               |             Type             | Default Value | Description                                                               |
| ----------------- | :--------------------------: | :-----------: | ------------------------------------------------------------------------- |
| port              |             int              |     25565     | The port for the server to listen on                                      |
| compression_level | int (0 to 9, -1 for default) |      -1       | The compression level for ZLib, 0 means none, 9 means best                |
| online_mode       |             bool             |     true      | Whether to check if the client is crack or not                            |
| address           |            string            |   127.0.0.1   | The IP address for the server to listen on                                |
| backlog           |             int              |      100      | The number of pending connections for the server to hold before accepting |
#### Display
| Key  | Type  | Default Value | Description                                                               |
| ---- | :---: | :-----------: | ------------------------------------------------------------------------- |
| chat | motd  |       /       | A message of the day in the form of a [Chat object](https://wiki.vg/Chat#Inheritance) |
#### Other
| Key      |   Type   | Default Value | Description                                                               |
| -------- | :------: | :-----------: | ------------------------------------------------------------------------- |
| loglevel | loglevel |      ALL      | Loglevel for the logger (ALL < DEBUG < INFO < WARN < ERROR < FATAL < OFF) |

## Contribution
A few instructions and informations on contributing to this project.

### Documentation
All of the code is documented and published using [doxygen](https://www.doxygen.nl/) on github pages [right here](https://lygaen.github.io/mineserver/) !
We are more than happy for you to provide feedback on the documentation and we aim for our code to be fully documented so that anyone can pick up the project. Contributing to the documentation of this project (and other projects in general) is a good way to familiarize with it and later contribute to it.
### Code
All of the contributed code on this project falls under our [license](#licensing). We use pull-requests from github to endorse / refuse code from other contributors. You can follow [Github's guide on creating pull-requests](https://docs.github.com/fr/pull-requests/collaborating-with-pull-requests/proposing-changes-to-your-work-with-pull-requests/creating-a-pull-request) to learn how to push code to this project.
We will be happy for you to participate in this project, and, even if you push code that is wrong or buggy, you shall only receive positive feedback from other contributors so that you can progress on your programmer's skills in general.
You will never be wrong to try to push new code onto this project and we will always be glad for you to do so !

## Libraries
Mineserver uses a set of libraries, but tries to be independant as
possible from them and uses only Open-Source libraries :
  - [Rapidjson](https://github.com/Tencent/rapidjson)
  - [Googletest](https://github.com/google/googletest)
  - [ZLib](https://github.com/madler/zlib)
  - [OpenSSL CMake Implementation](https://github.com/janbar/openssl-cmake)

## Licensing
This project uses [Apache License 2.0](https://choosealicense.com/licenses/apache-2.0/) for licensing. You can learn more information
by clicking on the above link.
