# 设置cmake最低版本要求
cmake_minimum_required(VERSION 4.0)

# 设置项目名称
project(CppLearning)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)

# 设置可执行文件输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 源文件
file(GLOB SOURCES "src/*.cpp")

add_executable(CppLearning ${SOURCES})
target_include_directories(CppLearning PRIVATE include)

# 测试文件
include(FetchContent)
FetchContent_Declare(
        doctest
        GIT_REPOSITORY https://github.com/doctest/doctest.git
        GIT_TAG        v2.4.12
)
FetchContent_MakeAvailable(doctest)


file(GLOB TEST_SOURCES "tests/test_spsc_queue.cpp")
add_executable(runTests ${TEST_SOURCES})
# 告诉编译器去include目录和doctest源文件的目录查找头文件
target_include_directories(runTests PRIVATE include ${doctest_SOURCE_DIR})

# 多线程测试
file (GLOB MT_TEST_SOURCES "tests/test_spsc_queue_mt.cpp")
add_executable(runTestsMT ${MT_TEST_SOURCES})
target_include_directories(runTestsMT PRIVATE include)