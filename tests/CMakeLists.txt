enable_testing(true)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(fpTest fpTest.cpp)
target_link_libraries(fpTest gtest gtest_main)
add_test(testFp fpTest)
