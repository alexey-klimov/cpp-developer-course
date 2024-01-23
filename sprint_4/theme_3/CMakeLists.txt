get_filename_component(CURRENT_DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
set(TARGET_PREFIX "${TARGET_PREFIX}${CURRENT_DIR_NAME}_")

add_subdirectory(lesson_2)
add_subdirectory(lesson_3)
