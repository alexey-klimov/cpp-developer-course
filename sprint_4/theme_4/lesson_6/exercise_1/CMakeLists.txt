cmake_minimum_required(VERSION 3.2)

get_filename_component(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project("${TARGET_PREFIX}${PROJECT_DIR}")

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/carpenter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/square_calculation.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/wall.cpp
)

set(HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/builder.h
    ${CMAKE_CURRENT_SOURCE_DIR}/carpenter.h
    ${CMAKE_CURRENT_SOURCE_DIR}/painter.h
    ${CMAKE_CURRENT_SOURCE_DIR}/square_calculation.h
    ${CMAKE_CURRENT_SOURCE_DIR}/wall.h
)

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})
