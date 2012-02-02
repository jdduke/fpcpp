enable_testing()
#find_package(GTest REQUIRED)

message(STATUS GTEST_ROOT=$ENV{GTEST_ROOT})
#include_directories(${GTEST_INCLUDE_DIRS} ${COMMON_INCLUDES})
include_directories($ENV{GTEST_ROOT}/include)
link_directories($ENV{GTEST_ROOT}/build)

set(PROJECT_TEST_NAME fpcppTest)

file(GLOB TEST_SRC_FILES *.cpp)
add_executable(${PROJECT_TEST_NAME} ${TEST_SRC_FILES})
#target_link_libraries(${PROJECT_TEST_NAME} ${GTEST_BOTH_LIBRARIES})
target_link_libraries(${PROJECT_TEST_NAME} gtest gtest_main)

add_test(test1 ${PROJECT_TEST_NAME})
