cmake_minimum_required(VERSION 3.10)
project(calc C)

file(GLOB_RECURSE SOURCE "src/*.c")

add_executable(${PROJECT_NAME} ${SOURCE})