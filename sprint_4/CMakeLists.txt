# Make prefix for targets from directory name
get_filename_component(TARGET_PREFIX ${CMAKE_CURRENT_SOURCE_DIR} NAME)
set(TARGET_PREFIX "${TARGET_PREFIX}_")

add_subdirectory(theme_3)
add_subdirectory(theme_4)
