cmake_minimum_required(VERSION 3.12)
project(xengine)
set(ENGINE_PROJECT_NAME xengine)

set(ENGINE_PUBLIC_EXTERNAL_INCLUDES
	includes/stb/stb_image.hpp
)
set(ENGINE_PUBLIC_INCLUDES
	includes/xengine/app.hpp
	includes/xengine/utils.hpp
	includes/xengine/audio.hpp
	includes/xengine/enviroment.hpp
	includes/xengine/rendering/renderer.hpp
	includes/xengine/rendering/window.hpp
	includes/xengine/rendering/camera.hpp
	includes/xengine/rendering/material.hpp
	includes/xengine/rendering/texture.hpp
	includes/xengine/rendering/mesh.hpp
	includes/xengine/rendering/transform.hpp
	includes/xengine/rendering/light.hpp
	includes/xengine/physics/rigidbody.hpp
	includes/xengine/input.hpp
	includes/xengine/input/keyboard.hpp
	includes/xengine/input/mouse.hpp
	includes/xengine/input/joystick.hpp
)
set(ENGINE_PRIVATE_INCLUDES
	includes/xengine/log.hpp
	includes/xengine/rendering/shader.hpp
)
set(ENGINE_PRIVATE_SOURCES
	src/xengine/log.cpp
	src/xengine/app.cpp
	src/xengine/utils.cpp
	src/xengine/audio.cpp
	src/xengine/enviroment.cpp
	src/xengine/rendering/renderer.cpp
	src/xengine/rendering/window.cpp
	src/xengine/rendering/shader.cpp
	src/xengine/rendering/camera.cpp
	src/xengine/rendering/texture.cpp
	src/xengine/rendering/mesh.cpp
	src/xengine/rendering/transform.cpp
	src/xengine/rendering/light.cpp
	src/xengine/physics/rigidbody.cpp
	src/xengine/input/keyboard.cpp
	src/xengine/input/mouse.cpp
	src/xengine/input/joystick.cpp
)
set(ENGINE_ALL
	${ENGINE_PRIVATE_SOURCES}
	${ENGINE_PRIVATE_INCLUDES}
	${ENGINE_PUBLIC_INCLUDES}
	${ENGINE_PUBLIC_EXTERNAL_INCLUDES}
)

add_library(${ENGINE_PROJECT_NAME} STATIC
	${ENGINE_ALL}
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES
	${ENGINE_ALL}
)

target_include_directories(${ENGINE_PROJECT_NAME} PUBLIC includes)
target_include_directories(${ENGINE_PROJECT_NAME} PUBLIC stb)
target_include_directories(${ENGINE_PROJECT_NAME} PRIVATE src)
target_compile_features(${ENGINE_PROJECT_NAME} PUBLIC cxx_std_17)

add_subdirectory(../external/glfw ${CMAKE_CURRENT_BINARY_DIR}/glfw)
target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE glfw)

add_subdirectory(../external/glad ${CMAKE_CURRENT_BINARY_DIR}/glad)
target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE glad)

add_subdirectory(../external/glm ${CMAKE_CURRENT_BINARY_DIR}/glm)
target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE glm)

add_subdirectory(../external/assimp ${CMAKE_CURRENT_BINARY_DIR}/assimp)
target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE assimp)

add_subdirectory(../external/portaudio ${CMAKE_CURRENT_BINARY_DIR}/portaudiod)
target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE ..\\xengine\\portaudiod\\portaudiod)

add_subdirectory(../external/libsndfile ${CMAKE_CURRENT_BINARY_DIR}/libsndfile)
target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE ..\\xengine\\libsndfile\\Release\\sndfile)

set(IMGUI_INCLUDES
	../external/imgui/imgui.h
	../external/imgui/backends/imgui_impl_glfw.h
	../external/imgui/backends/imgui_impl_opengl3.h
)
set(IMGUI_SOURCES
	../external/imgui/imgui.cpp
	../external/imgui/backends/imgui_impl_glfw.cpp
	../external/imgui/backends/imgui_impl_opengl3.cpp
	../external/imgui/imgui_draw.cpp
	../external/imgui/imgui_tables.cpp
	../external/imgui/imgui_widgets.cpp
	../external/imgui/imgui_demo.cpp
)
add_library(ImGui STATIC
	${IMGUI_INCLUDES}
	${IMGUI_SOURCES}
)
target_include_directories(ImGui PUBLIC ../external)
target_include_directories(ImGui PUBLIC ../external/imgui)
target_link_libraries(ImGui PRIVATE glad glfw)
target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE ImGui)
