# C++ and C project template

Project template for building C++ and C projects with CMake and various different compilers
and IDEs. Install [CMake](https://cmake.org/install/) first.

All builds displayed here are for Debug configurations. To build for release, replace
`Debug` with `Release` in the folder names and and add `-DCMAKE_BUILD_TYPE=Release` 
to the CMake build command.

## Windows - GCC

### Using MinGW Makefiles

Install [MSYS2](https://www.msys2.org/) first. All command line steps here were done
in MinGW64. Set up MinGW64:

```sh
pacman -Syuu
pacman -S git mingw-w64-x86_64-toolchain mingw-w64-x86_64-cmake
git config --global core.autocrlf true
```

Then build the application with

```cpp
mkdir build-MinGW64 && cd build-MinGW64
cmake -G "MinGW Makefiles" ..
cmake --build . -j
```

#### Using Visual Studio Code

Make sure you can build the application with MinGW64 like specified above. After that
there are good instructions on how to set up Visual Studio Code 
[here](https://code.visualstudio.com/docs/cpp/config-mingw). A workspace file is provided as well
to get started quickly.

#### Using Eclipse

Copy the `.project` and `.cproject` file found in `misc/eclipse` into the project root
and open the folder in Eclipse with `Open Project from Filesystem`. Build configurations and 
launch files for MinGW were provided. You have to generate the build system with CMake via 
command line first before you can Build, Run and Debug with Eclipse.

## Windows - MSVC

### Using Visual Studio

Install [Visual Studio](https://visualstudio.microsoft.com/).
You can generate Visual Studio project files with the following command 
(here, for Visual Studio 2019)

```cpp
mkdir Debug-VS2019
cd Debug-VS2019
cmake .. -G "Visual Studio 16 2019"
```

After that, a `.sln` project file is generated which you can open with Visual Studio 2019.

###  Using Visual Studio Code

Instructions can be found [here](https://code.visualstudio.com/docs/cpp/config-msvc).
Not tested yet.

## Windows - LLVM

### Using MinGW Makefiles

Install [LLVM](https://llvm.org/builds/) first.
Perform the same set-up as specified above for MinGW64.

Ensure you can call `clang --version` from the command line by adding the LLVM binary path
to the MinGW64 path as well, for example by adding the following line in the `~/.bashrc` file

```sh
export PATH=$PATH:"/c/Program Files/LLVM/bin"
```

If everything was set up correctly, you can build the application with `clang`

```cpp
mkdir build-MinGW64 && cd build-MinGW64
cmake -G "MinGW Makefiles" -DUSE_LLVM=ON ..
cmake --build . -j
```

## Linux - GCC

### Using GCC and Unix Makefiles

Instructions for Ubuntu, adapt accordingly
Install required packages and tools first

```cpp
sudo apt-get update
sudo apt-get install build-essential gdb
```

Build the project like this: 

```cpp
mkdir Debug-Unix
cd Debug-Unix
cmake .. 
cmake --build . -j
```

#### Using Eclipse

There are two Eclipse files `.project` and `.cproject` located inside the `misc/eclipse` folder.
IN the `.project` file, change the name to your project folder name. After that, copy both files
into the project root and open the folder in Eclipse with `Open Project from Filesystem`. Build
configurations and  launch files for Unix systems were provided. You have to generate the build
system with CMake via  command line first before you can Build, Run and Debug with Eclipse.

#### Using Visual Studio Code

Make sure you can build the application like specified above. After that
there are good instructions on how to set up Visual Studio Code 
[here](https://code.visualstudio.com/docs/cpp/config-linux)

## Linux - LLVM

Install [LLVM](https://apt.llvm.org/) first.

If everything was set up correctly, you can build the application with `clang`

```cpp
mkdir build && cd build
cmake -DUSE_LLVM=ON ..
cmake --build . -j
```
