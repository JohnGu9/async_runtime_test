cmake_minimum_required(VERSION 3.10)

project(async_runtime_test)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

macro(SET_OPTION option value)
  set(${option} ${value} CACHE INTERNAL "" FORCE)
endmacro()

# add lib
SET_OPTION(ASYNC_RUNTIME_WITH_IO ON)
add_subdirectory(async_runtime)
# add include path
include_directories(async_runtime/include)

# test globally
add_executable(test test/test.cpp)
target_link_libraries(test async_runtime)

# test case
if(WIN32)
add_definitions(/wd4100 /wd4458)
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

add_executable(test_basic_function test/test_basic/function.cpp)
target_link_libraries(test_basic_function async_runtime)

# fundamental
add_executable(test_fundamental_event_loop test/test_fundamental/event_loop.cpp)
target_link_libraries(test_fundamental_event_loop async_runtime)

add_executable(test_fundamental_future test/test_fundamental/future.cpp)
target_link_libraries(test_fundamental_future async_runtime)

add_executable(test_fundamental_stream test/test_fundamental/stream.cpp)
target_link_libraries(test_fundamental_stream async_runtime)

add_executable(test_fundamental_timer test/test_fundamental/timer.cpp)
target_link_libraries(test_fundamental_timer async_runtime)

add_executable(test_fundamental_file test/test_fundamental/file.cpp)
target_link_libraries(test_fundamental_file async_runtime)

# io
add_executable(test_io_udp test/test_io/udp.cpp)
target_link_libraries(test_io_udp async_runtime)

add_executable(test_io_tcp test/test_io/tcp.cpp)
target_link_libraries(test_io_tcp async_runtime)

# widget
add_executable(test_widget_stateless_widget test/test_widget/stateless_widget.cpp)
target_link_libraries(test_widget_stateless_widget async_runtime)

add_executable(test_widget_stateful_widget test/test_widget/stateful_widget.cpp)
target_link_libraries(test_widget_stateful_widget async_runtime)

add_executable(test_widget_logger_widget test/test_widget/logger_widget.cpp)
target_link_libraries(test_widget_logger_widget async_runtime)

add_executable(test_widget_multi_child_widget test/test_widget/multi_child_widget.cpp)
target_link_libraries(test_widget_multi_child_widget async_runtime)

add_executable(test_widget_named_multi_child_widget test/test_widget/named_multi_child_widget.cpp)
target_link_libraries(test_widget_named_multi_child_widget async_runtime)

add_executable(test_widget_value_listenable_builder test/test_widget/value_listenable_builder.cpp)
target_link_libraries(test_widget_value_listenable_builder async_runtime)

add_executable(test_widget_future_builder test/test_widget/future_builder.cpp)
target_link_libraries(test_widget_future_builder async_runtime)

add_executable(test_widget_stream_builder test/test_widget/stream_builder.cpp)
target_link_libraries(test_widget_stream_builder async_runtime)

# widget key
add_executable(test_widget_key_value_key test/test_widget_key/value_key.cpp)
target_link_libraries(test_widget_key_value_key async_runtime)