cmake_minimum_required(VERSION 3.12)
project(XEngine)
set(PROJECT_NAME XEngine)

add_subdirectory(xengine)
add_subdirectory(xeditor)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT xeditor)