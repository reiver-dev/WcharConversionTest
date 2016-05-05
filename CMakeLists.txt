cmake_minimum_required(VERSION 3.4)
project(Convert)

file(GLOB HEADERS "src/*.hpp")
file(GLOB SOURCE "src/*.cpp")

add_executable(convert ${SOURCE} ${HEADERS})
