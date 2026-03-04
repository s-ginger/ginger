cmake_minimum_required(VERSION 3.10)
project(calc CXX)

file(GLOB_RECURSE SOURCE "src/*.cc")

if(MSVC)
    add_compile_options(/W4 /permissive-)
    # /W4 — почти все предупреждения
    # /permissive- — строгий стандарт C++
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_executable(${PROJECT_NAME} ${SOURCE})
target_include_directories(${PROJECT_NAME} PRIVATE include)