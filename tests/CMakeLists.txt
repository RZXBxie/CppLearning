# 公共依赖
set(TEST_INC ../include ${doctest_SOURCE_DIR})

# SPSC
add_executable(test_spsc  test_spsc_queue.cpp)
target_include_directories(test_spsc PRIVATE ${TEST_INC})
target_link_libraries(test_spsc doctest::doctest)

# MPMC
add_executable(test_mpmc  test_mpmc_queue.cpp)
target_include_directories(test_mpmc PRIVATE ${TEST_INC})
target_link_libraries(test_mpmc doctest::doctest)

# 以后加测试只需再写三行：
# add_executable(test_xxx test_xxx.cpp)
# target_include_directories(test_xxx PRIVATE ${TEST_INC})
# target_link_libraries(test_xxx doctest::doctest)