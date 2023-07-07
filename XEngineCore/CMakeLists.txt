cmake_minimum_required(VERSION 3.12)
project(XEngineCore)
set(ENGINE_PROJECT_NAME XEngineCore)

set(ENGINE_PUBLIC_INCLUDES
	includes/XEngineCore/App.hpp
	includes/XEngineCore/Log.hpp
	includes/XEngineCore/Event.hpp
)
set(ENGINE_PRIVATE_INCLUDES
	src/XEngineCore/Window.hpp
)
set(ENGINE_PRIVATE_SOURCES
	src/XEngineCore/App.cpp
	src/XEngineCore/Window.cpp
)

add_library(${ENGINE_PROJECT_NAME} STATIC
	${ENGINE_PRIVATE_SOURCES}
	${ENGINE_PRIVATE_INCLUDES}
	${ENGINE_PUBLIC_INCLUDES}
)

target_include_directories(${ENGINE_PROJECT_NAME} PUBLIC includes)
target_include_directories(${ENGINE_PROJECT_NAME} PRIVATE src)
target_compile_features(${ENGINE_PROJECT_NAME} PUBLIC cxx_std_17)

add_subdirectory(../external/glfw ${CMAKE_CURRENT_BINARY_DIR}/glfw)
target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE glfw)

add_subdirectory(../external/spdlog ${CMAKE_CURRENT_BINARY_DIR}/spdlog)
target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE spdlog)

add_subdirectory(../external/glad ${CMAKE_CURRENT_BINARY_DIR}/glad)
target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE glad)