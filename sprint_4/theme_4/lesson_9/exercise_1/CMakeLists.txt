cmake_minimum_required(VERSION 3.2)

get_filename_component(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project("${TARGET_PREFIX}${PROJECT_DIR}")

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/document.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/read_input_functions.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/request_queue.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/search_server.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/string_processing.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/test_example_functions.cpp
)

set(HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/document.h
    ${CMAKE_CURRENT_SOURCE_DIR}/paginator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/read_input_functions.h
    ${CMAKE_CURRENT_SOURCE_DIR}/request_queue.h
    ${CMAKE_CURRENT_SOURCE_DIR}/search_server.h
    ${CMAKE_CURRENT_SOURCE_DIR}/string_processing.h
    ${CMAKE_CURRENT_SOURCE_DIR}/test_example_functions.h
)

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})
