cmake_minimum_required(VERSION 3.6)
project(Glad C)

include_directories(include)

set(GLAD_SRC src/gl.c)

if (PLATFORM_LINUX)
list(APPEND GLAD_SRC src/glx.c)
endif()

if (PLATFORM_WINDOWS)
list(APPEND GLAD_SRC src/wgl.c)
endif()

add_library(glad ${GLAD_SRC})