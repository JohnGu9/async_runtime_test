cmake_minimum_required(VERSION 3.10)

project(async_runtime_test)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# add lib
add_subdirectory(async_runtime)
# add include path
include_directories(${ASYNC_RUNTIME_INCLUDE_DIR})
message(STATUS "Using Async Runtime include: ${ASYNC_RUNTIME_INCLUDE_DIR}")
message(STATUS "Using Async Runtime libraries: async_runtime")


# test globally
add_executable(test test/test.cpp)
target_link_libraries(test async_runtime)

# test case
if(WIN32)
add_definitions(/W4 /wd4100 /wd4458)
else()
add_definitions(-Wall)
endif()

# basic
add_executable(test_basic_callback test/test_basic/callback.cpp)
target_link_libraries(test_basic_callback async_runtime)

add_executable(test_basic_lock test/test_basic/lock.cpp)
target_link_libraries(test_basic_lock async_runtime)

add_executable(test_basic_container test/test_basic/container.cpp)
target_link_libraries(test_basic_container async_runtime)

add_executable(test_basic_string test/test_basic/string.cpp)
target_link_libraries(test_basic_string async_runtime)

# fundamental
add_executable(test_fundamental_timer test/test_fundamental/timer.cpp)
target_link_libraries(test_fundamental_timer async_runtime)

add_executable(test_fundamental_file test/test_fundamental/file.cpp)
target_link_libraries(test_fundamental_file async_runtime)

add_executable(test_fundamental_logger test/test_fundamental/logger.cpp)
target_link_libraries(test_fundamental_logger async_runtime)

add_executable(test_fundamental_http test/test_fundamental/http.cpp)
target_link_libraries(test_fundamental_http async_runtime)

# widget
add_executable(test_widget_my_widget test/test_widget/my_widget.cpp)
target_link_libraries(test_widget_my_widget async_runtime)

add_executable(test_widget_stateless_widget test/test_widget/stateless_widget.cpp)
target_link_libraries(test_widget_stateless_widget async_runtime)

add_executable(test_widget_stateful_widget test/test_widget/stateful_widget.cpp)
target_link_libraries(test_widget_stateful_widget async_runtime)

add_executable(test_widget_multi_child_widget test/test_widget/multi_child_widget.cpp)
target_link_libraries(test_widget_multi_child_widget async_runtime)

add_executable(test_widget_named_multi_child_widget test/test_widget/named_multi_child_widget.cpp)
target_link_libraries(test_widget_named_multi_child_widget async_runtime)

add_executable(test_widget_process test/test_widget/process.cpp)
target_link_libraries(test_widget_process async_runtime)

add_executable(test_widget_value_listenable_builder test/test_widget/value_listenable_builder.cpp)
target_link_libraries(test_widget_value_listenable_builder async_runtime)

add_executable(test_widget_future_builder test/test_widget/future_builder.cpp)
target_link_libraries(test_widget_future_builder async_runtime)
