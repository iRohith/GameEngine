cmake_minimum_required(VERSION 3.6)
project(Tests CXX)

add_executable(LogTest LogTest.cpp)
add_dependencies(LogTest GameEngine)
target_link_libraries(LogTest PUBLIC GameEngine)

add_executable(AppInitTest AppInitTest.cpp)
add_dependencies(AppInitTest GameEngine)
target_link_libraries(AppInitTest PUBLIC GameEngine)

add_executable(RawTriangle RawTriangle.cpp)
add_dependencies(RawTriangle GameEngine)
target_link_libraries(RawTriangle PUBLIC GameEngine)

add_executable(RawCube RawCube.cpp)
add_dependencies(RawCube GameEngine)
target_link_libraries(RawCube PUBLIC GameEngine)

add_executable(RawTexturedCube RawTexturedCube.cpp)
add_dependencies(RawTexturedCube GameEngine)
target_link_libraries(RawTexturedCube PUBLIC GameEngine)

add_executable(GLTest GLTest.cpp)
add_dependencies(GLTest glad)
include_directories(
        ThirdParty/glad/include
    )
target_link_libraries(GLTest PUBLIC X11 glad ${CMAKE_DL_LIBS})

add_executable(GLTriangle GLTriangle.cpp)
add_dependencies(GLTriangle glad)
include_directories(
        ThirdParty/glad/include
    )
target_link_libraries(GLTriangle PUBLIC X11 glad ${CMAKE_DL_LIBS})

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR}/bin)