cmake_minimum_required(VERSION 3.12)
project(XEngine)
set(PROJECT_NAME XEngine)

add_subdirectory(XEngineCore)
add_subdirectory(XEngineEditor)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT XEngineEditor)