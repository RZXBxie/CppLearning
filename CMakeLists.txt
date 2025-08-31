cmake_minimum_required(VERSION 4.0)
project(CppLearning)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# ---------------- 主程序 ----------------
file(GLOB SOURCES "src/*.cpp")

add_executable(CppLearning ${SOURCES})
target_include_directories(CppLearning PRIVATE include)

# ---------------- doctest ----------------
include(FetchContent)
FetchContent_Declare(
        doctest
        GIT_REPOSITORY https://github.com/doctest/doctest.git
        GIT_TAG        v2.4.12
)
FetchContent_MakeAvailable(doctest)

# 将测试文件交出去
add_subdirectory(tests)
