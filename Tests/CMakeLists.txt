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

add_executable(RawTexture RawTexture.cpp)
add_dependencies(RawTexture GameEngine)
target_link_libraries(RawTexture PUBLIC GameEngine)

add_executable(FlappyBird FlappyBird/main.cpp)
add_dependencies(FlappyBird GameEngine)
target_link_libraries(FlappyBird PUBLIC GameEngine)

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR}/bin)