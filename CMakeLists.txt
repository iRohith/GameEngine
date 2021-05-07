cmake_minimum_required(VERSION 3.6)
project(GameEngine CXX)

################## Config ######################

set(ENGINE_STATIC_BUILD ON)
set(COMPILE_TESTS ON)

set(DISABLE_USER_LOGGING OFF)
set(DISABLE_ENGINE_LOGGING OFF)
set(NO_EXTERNAL_LOGGER ON)

set(RENDERER_OPENGL ON)
SET(RENDERER_DIRECTX OFF)

################## Config End ###################

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(GE_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


if (NOT DEFINED PLATFORM_SET)
if(ANDROID)
    set(PLATFORM_ANDROID ON)
    set(PLATFORM_MOBILE ON)
    add_compile_definitions(PLATFORM_MOBILE PLATFORM_ANDROID)
elseif(IOS)
    set(PLATFORM_IOS ON)
    set(PLATFORM_MOBILE ON)
    add_compile_definitions(PLATFORM_MOBILE PLATFORM_IOS)
elseif (WIN32)
    set(PLATFORM_WINDOWS ON)
    set(PLATFORM_DESKTOP ON)
    add_compile_definitions(PLATFORM_DESKTOP PLATFORM_WINDOWS)
elseif(UNIX AND NOT APPLE)
    set(PLATFORM_LINUX ON)
    set(PLATFORM_DESKTOP ON)
    add_compile_definitions(PLATFORM_DESKTOP PLATFORM_LINUX)
elseif(APPLE)
    set(PLATFORM_APPLE ON)
    set(PLATFORM_DESKTOP ON)
    add_compile_definitions(PLATFORM_DESKTOP PLATFORM_APPLE)
else()
message(FATAL_ERROR "Unsupported platform. Aborting..." )
endif()
set(PLATFORM_SET ON)
add_compile_definitions(PLATFORM_SET)
endif()

if (RENDERER_OPENGL)
    add_compile_definitions(RENDERER_OPENGL)
    add_subdirectory(ThirdParty/glad)
    include_directories(
        ThirdParty/glad/include
    )
endif()
if (RENDERER_DIRECTX)
    add_compile_definitions(RENDERER_DIRECTX)
endif()

if (DISABLE_USER_LOGGING)
    add_compile_definitions(DISABLE_USER_LOGGING)
endif()
if (DISABLE_ENGINE_LOGGING)
    add_compile_definitions(DISABLE_ENGINE_LOGGING)
endif()
if (NO_EXTERNAL_LOGGER)
    add_compile_definitions(NO_EXTERNAL_LOGGER)
endif()

add_subdirectory(ThirdParty/stb)
include_directories(ThirdParty/stb)

set(SRC
    "src/core/GlobalStore.cpp"
    "src/core/Log.cpp"
    "src/core/App.cpp"
    "src/core/Window.cpp"
    "src/core/EventHandler.cpp"
    "src/core/Input.cpp"
    "src/core/Time.cpp"

    "src/graphics/GraphicsContext.cpp"
    "src/graphics/Buffer.cpp"
    "src/graphics/BufferSpec.cpp"
    "src/graphics/Renderer.cpp"
    "src/graphics/Shader.cpp"
    "src/graphics/Texture.cpp"
)

if (ENGINE_STATIC_BUILD)
    add_compile_definitions(ENGINE_STATIC_BUILD)
    add_library(GameEngine STATIC ${SRC})
else()
    add_library(GameEngine SHARED ${SRC})
    target_compile_definitions(GameEngine PUBLIC GAME_ENGINE_IMPL)
endif()

target_link_libraries(GameEngine PRIVATE STB)

if (PLATFORM_WINDOWS)
target_link_libraries(GameEngine PRIVATE winmm gdi32)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        target_compile_definitions(GameEngine PUBLIC /wd4267 /wd4305)
    endif()
else()
target_link_libraries(GameEngine PRIVATE X11)
endif()

if (RENDERER_OPENGL)
    target_link_libraries(GameEngine PRIVATE glad ${CMAKE_DL_LIBS})
    if (PLATFORM_WINDOWS)
        target_link_libraries(GameEngine PRIVATE opengl32)
    endif()
endif()

target_include_directories(GameEngine PUBLIC 
    "include"
    "ThirdParty/glm"
    "ThirdParty/stb"
)

if (NOT NO_EXTERNAL_LOGGER AND (NOT DISABLE_USER_LOGGING OR NOT DISABLE_ENGINE_LOGGING))
    target_include_directories(GameEngine PUBLIC "ThirdParty/spdlog/include")
endif()

if (COMPILE_TESTS)
add_subdirectory(Tests)
endif()