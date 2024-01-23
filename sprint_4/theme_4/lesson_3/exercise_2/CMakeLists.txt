cmake_minimum_required(VERSION 3.2)

get_filename_component(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project("${TARGET_PREFIX}${PROJECT_DIR}")

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

set(HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/painter.h
    ${CMAKE_CURRENT_SOURCE_DIR}/wall.h
)

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})
