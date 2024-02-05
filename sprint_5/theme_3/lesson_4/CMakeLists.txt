cmake_minimum_required(VERSION 3.2)

get_filename_component(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${PROJECT_DIR})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")

get_filename_component(CURRENT_DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
set(TARGET_PREFIX "${TARGET_PREFIX}${CURRENT_DIR_NAME}_")

file(GLOB SOURCES *.cpp)
foreach(SOURCE ${SOURCES})
    get_filename_component(EXECUTABLE ${SOURCE} NAME_WE)
    set(EXECUTABLE ${TARGET_PREFIX}${EXECUTABLE})
    add_executable(${EXECUTABLE} ${SOURCE})
endforeach()
