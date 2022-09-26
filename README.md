# Example SDL2 Application

This repo serves as a template to build an SDL2-based game/application written in C++ that utilizes CMake as the build system and vcpkg as the dependency manager.

The code for our "game" is a very basic "Hello World!" that does the following:
* Initializes SDL2
* Loads an image
* Renders the image
* Handles input (just the `SDL_WINDOWEVENT_CLOSE` event)

Essentially, just the basics to show that SDL2 is working correctly.

## License

All code and resources are licensed under the "Unlicense", which is described in the file `LICENSE`.

## Required Software

### Windows
* Microsoft Visual Studio 2022 - or at least just the build tools if you want to use a different IDE/editor (like VS Code)
    * Be sure to install the [`C++ CMake tools for Windows` from the Visual Studio Installer](https://learn.microsoft.com/en-us/cpp/build/cmake-projects-in-visual-studio)
* vcpkg
    * This template assumes that it is installed at `C:\vcpkg` - this can be changed in `CMakeLists.txt`.

### macOS
* Xcode Command Line Tools (via `xcode-select --install`)
* vcpkg
    * This template assumes that it is installed at `~/vcpkg` - this can be changed in `CMakeLists.txt`.
* CMake
    * Be sure to ***NOT*** install the Homebrew version. Install the version from Kitware, as that supports building both Apple Silicon and Intel binaries.
* Ninja (through Homebrew)

### Linux
* GCC or Clang
* vcpkg
    * This template assumes that it is installed at `~/vcpkg` - this can be changed in `CMakeLists.txt`.
* CMake
* Ninja

### Emscripten
* Emscripten SDK (emsdk)
    * This template assumes that it is installed at `~/emsdk` (or `C:\emsdk` on Windows) - this can be changed in `CMakeLists.txt`.
* Ninja

## How to build

It is quite easy to build from VS Code with the [`C/C++ Extension Pack`](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools-extension-pack)

Otherwise, on the command line, it is just a little bit verbose.

### Windows

`cmake -DCMAKE_TOOLCHAIN_FILE:STRING=C:/vcpkg/scripts/buildsystems/vcpkg.cmake -S . -B build -G "Visual Studio 17 2022" -T host=x64 -A x64` (I can't really verify this because I use VS Code on Windows)

### macOS

* `cmake -G Ninja -b build -DCMAKE_TOOLCHAIN_FILE=~/vcpkg/scripts/buildsystems/vcpkg.cmake ./`
* `cd build && ninja`

In order to compile an Intel binary, just run `arch -x86_64` in front of the above cmake command.

### Linux

* `cmake -G Ninja -S . -B build -DCMAKE_TOOLCHAIN_FILE=~/vcpkg/scripts/buildsystems/vcpkg.cmake ./`
* `cd build/ && ninja`


## Configuring Visual Studio Code

Create a file in the `.vscode` directory called `settings.json`, you will need to configure this per platform you are developing on.

For all platforms, you will probably want to debug your game within VS Code, so add this snippet:

```
    "cmake.debugConfig": {
        "cwd": "${workspaceFolder}/build",
    },
```

### Windows

Add this snippet so the CMake Tools extension knows where to find vcpkg:

```
    "cmake.configureSettings": {
        "CMAKE_TOOLCHAIN_FILE":
            "C:/vcpkg/scripts/buildsystems/vcpkg.cmake"
    },
```

### macOS

Add this snippet so the CMake Tools extension knows where to find vcpkg:

```
    "cmake.configureSettings": {
        "CMAKE_TOOLCHAIN_FILE": "~/vcpkg/scripts/buildsystems/vcpkg.cmake"
    },
```

### Linux

Add this snippet so the CMake Tools extension knows where to find vcpkg:

```
    "cmake.configureSettings": {
        "CMAKE_TOOLCHAIN_FILE": "~/vcpkg/scripts/buildsystems/vcpkg.cmake"
    },
```

### Emscripten

Add this snippet so the CMake Tools extensions knows where to find vcpkg and Emscripten (tools and libraries)

```
    "cmake.configureSettings": {
        "CMAKE_TOOLCHAIN_FILE": "~/vcpkg/scripts/buildsystems/vcpkg.cmake",
        "VCPKG_CHAINLOAD_TOOLCHAIN_FILE": "~/emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake"
    },

    "cmake.additionalKits": [
        "${workspaceFolder}/emscripten-cmake-kit.json"
    ],
```
