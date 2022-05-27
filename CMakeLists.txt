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

#object
add_executable(test_object_container test/test_object/container.cpp)
target_link_libraries(test_object_container async_runtime_object)

add_executable(test_object_string test/test_object/string.cpp)
target_link_libraries(test_object_string async_runtime_object)

add_executable(test_object_function test/test_object/function.cpp)
target_link_libraries(test_object_function async_runtime_object)

# utilities
add_executable(test_utilities_value_notifier test/test_utilities/value_notifier.cpp)
target_link_libraries(test_utilities_value_notifier async_runtime_utilities)

add_executable(test_utilities_lock test/test_utilities/lock.cpp)
target_link_libraries(test_utilities_lock async_runtime_fundamental)

# fundamental
add_executable(test_fundamental_event_loop test/test_fundamental/event_loop.cpp)
target_link_libraries(test_fundamental_event_loop async_runtime_fundamental)

add_executable(test_fundamental_future test/test_fundamental/future.cpp)
target_link_libraries(test_fundamental_future async_runtime_fundamental)

add_executable(test_fundamental_stream test/test_fundamental/stream.cpp)
target_link_libraries(test_fundamental_stream async_runtime_fundamental)

add_executable(test_fundamental_timer test/test_fundamental/timer.cpp)
target_link_libraries(test_fundamental_timer async_runtime_fundamental)

add_executable(test_fundamental_file test/test_fundamental/file.cpp)
target_link_libraries(test_fundamental_file async_runtime_fundamental)

# io
add_executable(test_io_udp test/test_io/udp.cpp)
target_link_libraries(test_io_udp async_runtime_fundamental)

add_executable(test_io_tcp test/test_io/tcp.cpp)
target_link_libraries(test_io_tcp async_runtime_fundamental)

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

add_executable(test_widget_key_global_key test/test_widget_key/global_key.cpp)
target_link_libraries(test_widget_key_global_key async_runtime)
