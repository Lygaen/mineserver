# Installation
[TOC]

## Step 1 : CMake Installation {#cmake_installation}
Minserver tries to have as few steps as possible for installation.
You don't need to install anything other than CMake on your local machine. Install it from the [website](https://cmake.org/) (on windows or other) or from your favorite package manager.

## Step 2 : Make/GCC Installation
Any C/C++ compiler would suffice, but the guide will follow the installation of GCC and G++.
Make is needed in order to work with CMake.

### Windows
It is a bit of a headache to make GCC and make work on Windows. It is advised to use [WSL 2](https://learn.microsoft.com/fr-fr/windows/wsl/install) on Windows and follow the Linux-side guide. If you still want it
to work on Windows, here is a step by step guide on installing MinGW :
 - Download [MinGW from winlibs](https://winlibs.com/)
 - Unzip the downloaded file into a folder (Like `C:\MinGW`)
 - [Add the folder `<Install Path>\bin\` to your path](https://linuxhint.com/add-directory-to-path-environment-variables-windows/) (so if you installed in `C:\MinGW`, you should add `C:\MinGW\bin`)

If you did nothing wrong, the following command should not error in your terminal :
```cmd
mingw32-make -v
```

Now you should be ready to got onto the next step.

### Linux
You can install using your favorite package manager gcc, g++ and make.
On some distros, all the necessary tools are already installed. Please refer to your distro's documentation for the installation. When finished, you can check versions with :
```sh
make -v
gcc -v
g++ -v
```
None of the above should fail.

## Step 3 : Cloning
Open a terminal and navigate to a folder where you would want to store the mineserver project.
You can then clone this repository and its submodules using [Git](https://git-scm.com/downloads) :
```sh
git clone --recurse-submodules https://github.com/Lygaen/mineserver/
```
There should be no problem if you are cloning on the default master branch. If a problem happens or you don't have / don't want to have git on your system, try to use [releases](https://github.com/Lygaen/mineserver/releases).
Know that if you clone using the zip button on GitHub, it will not clone the submodules. In that case, you will need to have [Git](https://git-scm.com/downloads) installed and run the following command in your directory :
```sh
git submodule update --init
```

## Step 4 : Configuring using CMake
### GUI
If you installed CMake as described on [step 1](#cmake_installation), you should have CMake GUI (`cmake-gui` in the terminal) installed on your system.

We will be configuring it :
 - Select the cloned / downloaded source code at the top
 - Select where to build the executable at
 - Now a lot of red variables should appear in a table
 - Configure the variable starting with `MINESERVER` (refer to [this](https://lygaen.github.io/mineserver/md_docs_2__2__CONFIG.html#cmake_definitions))
 - Press `Configure` then `Generate`

### Command Line
Now that you have the source code for mineserver, you can build it using your previously-installed cmake, using a build directory :
```sh
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
```
You can add `-D<YOUR OPTION>=<VALUE>` to the cmake command before the `..` to modify build parameters for Mineserver (refer to [this]([#cmake](https://lygaen.github.io/mineserver/md_docs_2__2__CONFIG.html#cmake_definitions))).

## Step 5 : Compiling
Open a terminal in the build folder or reuse the terminal in the previous steps, running the following commands. It should start building the executable. If any errors happened, please check that you did all of the previous steps or make an issue on Github, we will be happy to help !
### Windows
```
mingw32-make all
```
### Linux
```
make all
```
