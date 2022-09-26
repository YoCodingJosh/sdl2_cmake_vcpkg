cmake_minimum_required(VERSION 3.16)
project(AwesomeGame VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS OFF)

if (UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif()

# Set up vcpkg and Emscripten toolchain files
if (WIN32)
    set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake")
    if (${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
        set(VCPKG_CHAINLOAD_TOOLCHAIN_FILE "c:/emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake")
    endif()
else()
    set(CMAKE_TOOLCHAIN_FILE "~/vcpkg/scripts/buildsystems/vcpkg.cmake")
    if (${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
        set(VCPKG_CHAINLOAD_TOOLCHAIN_FILE "~/emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake")
    endif()
endif()

# Use the bundled SDL2 libraries with Emscripten
if(${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
    set(CMAKE_C_COMPILER "emcc")
    set(CMAKE_CXX_COMPILER "em++")
    set(USE_FLAGS "-s USE_SDL=2 -s USE_FREETYPE=1 -s USE_SDL_TTF=2 -s USE_SDL_GFX=2 -s USE_SDL_IMAGE=2 -sNO_DISABLE_EXCEPTION_CATCHING")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${USE_FLAGS} -pthread")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${USE_FLAGS} -pthread")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${USE_FLAGS} -pthread --use-preload-plugins -s SDL2_IMAGE_FORMATS='[\"png\"]' --preload-file assets")
    set(CMAKE_EXECUTABLE_SUFFIX .html)
else()
    find_package(SDL2 REQUIRED)
    find_package(SDL2_ttf CONFIG REQUIRED)
    find_package(sdl2-gfx CONFIG REQUIRED)
    find_package(sdl2-image CONFIG REQUIRED)
endif()

# Remove the command prompt window on Windows and set the Windows SDK version
if (WIN32)
    set (CMAKE_SYSTEM_VERSION 10.0.22621.0)
    add_executable(AwesomeGame WIN32 "")
else()
    add_executable(AwesomeGame "")
endif()

target_sources(AwesomeGame
    PRIVATE
    src/Main.cpp
)

# Copy the game assets to the build directory, so that we can debug the game easily
file(COPY assets/ DESTINATION assets/)

if (EMSCRIPTEN)
    # Emscripten is first because we can use Emscripten on Windows, Linux, and macOS, which will cause the other if statements to be true.
    target_link_libraries(AwesomeGame PRIVATE ${SDL2_LIBRARIES} ${FREETYPE_LIBRARIES})
elseif (APPLE)
    # This is for macOS
    # TODO: Handle iOS.
    target_link_libraries(AwesomeGame PRIVATE SDL2::SDL2 SDL2::SDL2main SDL2_ttf::SDL2_ttf-static SDL2::SDL2_gfx SDL2::SDL2_image "-framework CoreFoundation")
    set_target_properties(AwesomeGame PROPERTIES LINK_FLAGS "-Wl,-F/Library/Frameworks")
elseif (WIN32)
    target_link_libraries(AwesomeGame PRIVATE SDL2::SDL2 SDL2::SDL2main SDL2_ttf::SDL2_ttf SDL2::SDL2_gfx SDL2::SDL2_image "Shlwapi.lib")
elseif (LINUX)
    target_link_libraries(AwesomeGame PRIVATE SDL2::SDL2 SDL2::SDL2main SDL2_ttf::SDL2_ttf SDL2::SDL2_gfx SDL2::SDL2_image)
endif ()
