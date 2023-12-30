cmake_minimum_required(VERSION 3.12)
project(XEngineProject)
set(PROJECT_NAME XEngineProject)

add_subdirectory(XEngine)
add_subdirectory(XEngineEditor)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT XEngineEditor)