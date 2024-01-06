cmake_minimum_required(VERSION 3.12)
project(XEngine)
set(ENGINE_PROJECT_NAME XEngine)

set(ENGINE_PUBLIC_EXTERNAL_INCLUDES
	includes/stb/stb_image.hpp
)
set(ENGINE_PUBLIC_INCLUDES
	includes/XEngine/App.hpp
	includes/XEngine/Utils.hpp
	includes/XEngine/Audio.hpp
	includes/XEngine/Rendering/Renderer.hpp
	includes/XEngine/Rendering/Window.hpp
	includes/XEngine/Rendering/Camera.hpp
	includes/XEngine/Rendering/Material.hpp
	includes/XEngine/Rendering/Texture.hpp
	includes/XEngine/Rendering/Mesh.hpp
	includes/XEngine/Rendering/Transform.hpp
	includes/XEngine/Rendering/Light.hpp
	includes/XEngine/Input.hpp
	includes/XEngine/Input/Keyboard.hpp
	includes/XEngine/Input/Mouse.hpp
	includes/XEngine/Input/Joystick.hpp
)
set(ENGINE_PRIVATE_INCLUDES
	includes/XEngine/Log.hpp
	includes/XEngine/Rendering/Shader.hpp
)
set(ENGINE_PRIVATE_SOURCES
	src/XEngine/Log.cpp
	src/XEngine/App.cpp
	src/XEngine/Utils.cpp
	src/XEngine/Audio.cpp
	src/XEngine/Rendering/Renderer.cpp
	src/XEngine/Rendering/Window.cpp
	src/XEngine/Rendering/Shader.cpp
	src/XEngine/Rendering/Camera.cpp
	src/XEngine/Rendering/Texture.cpp
	src/XEngine/Rendering/Mesh.cpp
	src/XEngine/Rendering/Transform.cpp
	src/XEngine/Rendering/Light.cpp
	src/XEngine/Input/Keyboard.cpp
	src/XEngine/Input/Mouse.cpp
	src/XEngine/Input/Joystick.cpp
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
target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE ..\\XEngine\\portaudiod\\portaudiod)

add_subdirectory(../external/libsndfile ${CMAKE_CURRENT_BINARY_DIR}/libsndfile)
target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE ..\\XEngine\\libsndfile\\Release\\sndfile)

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
)
add_library(ImGui STATIC
	${IMGUI_INCLUDES}
	${IMGUI_SOURCES}
)
target_include_directories(ImGui PUBLIC ../external)
target_include_directories(ImGui PUBLIC ../external/imgui)
target_link_libraries(ImGui PRIVATE glad glfw)
target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE ImGui)
