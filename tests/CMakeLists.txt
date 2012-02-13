enable_testing(true)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

set(PROJECT_TEST_NAME fpcppTest)
file(GLOB TEST_SRC_FILES *.cpp)

add_executable(${PROJECT_TEST_NAME} ${TEST_SRC_FILES})
target_link_libraries(${PROJECT_TEST_NAME} gtest gtest_main)
add_test(test1 ${PROJECT_TEST_NAME})
