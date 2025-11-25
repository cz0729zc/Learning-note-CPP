cmake_minimum_required(VERSION 3.10.0)
project(Project1 VERSION 0.1.0 LANGUAGES C CXX)

# 包含头文件目录
include_directories(include)

# 自动收集src目录下所有cpp文件
file(GLOB_RECURSE SRC_FILES src/*.cpp)

add_executable(Project1 main.cpp ${SRC_FILES})

