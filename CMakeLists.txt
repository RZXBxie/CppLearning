# 设置cmake最低版本要求
cmake_minimum_required(VERSION 4.0)

# 设置项目名称
project(CppLearning)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)

# 设置可执行文件输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 指定源文件目录
file(GLOB SOURCES "src/*.cpp")

add_executable(CppLearning ${SOURCES})