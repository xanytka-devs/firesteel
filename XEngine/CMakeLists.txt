cmake_minimum_required(VERSION 3.12)
project(XEngine)
set(ENGINE_PROJECT_NAME XEngine)

set(ENGINE_PUBLIC_INCLUDES
	includes/XEngine/App.hpp
	includes/XEngine/Log.hpp
	includes/XEngine/Event.hpp
	includes/XEngine/Components/Transform.hpp
	includes/XEngine/CES.hpp
	includes/XEngine/Camera.hpp
	includes/XEngine/Input/Keys.hpp
	includes/XEngine/Input/Input.hpp
	includes/XEngine/Input/Cursor.hpp
	includes/XEngine/ResLoader.hpp
	includes/XEngine/SceneManager.hpp
	includes/stb/stb_image.hpp
)
set(ENGINE_PRIVATE_INCLUDES
	src/XEngine/Rendering/OpenGL/Window.hpp
	src/XEngine/UI/TUI.hpp
	src/XEngine/Rendering/OpenGL/ShaderProgram.hpp
	src/XEngine/Rendering/OpenGL/VertexBuffer.hpp
	src/XEngine/Rendering/OpenGL/VertexArray.hpp
	src/XEngine/Rendering/OpenGL/IndexBuffer.hpp
	src/XEngine/Rendering/OpenGL/Renderer.hpp
	src/XEngine/Rendering/OpenGL/Texture.hpp
	src/XEngine/Rendering/OpenGL/Material.hpp
	src/XEngine/Rendering/OpenGL/Mesh.hpp
)
set(ENGINE_PRIVATE_SOURCES
	src/XEngine/App.cpp
	src/XEngine/Rendering/OpenGL/Window.cpp
	src/XEngine/Input/Input.cpp
	src/XEngine/Input/Cursor.cpp
	src/XEngine/CES.cpp
	src/XEngine/Components/Transform.cpp
	src/XEngine/Camera.cpp
	src/XEngine/UI/TUI.cpp
	src/XEngine/ResLoader.cpp
	src/XEngine/Rendering/OpenGL/ShaderProgram.cpp
	src/XEngine/Rendering/OpenGL/VertexBuffer.cpp
	src/XEngine/Rendering/OpenGL/VertexArray.cpp
	src/XEngine/Rendering/OpenGL/IndexBuffer.cpp
	src/XEngine/Rendering/OpenGL/Renderer.cpp
	src/XEngine/Rendering/OpenGL/Texture.cpp
	src/XEngine/Rendering/OpenGL/Material.cpp
	src/XEngine/Rendering/OpenGL/Mesh.cpp
	src/XEngine/SceneManager.cpp
)
set(ENGINE_ALL
	${ENGINE_PRIVATE_SOURCES}
	${ENGINE_PRIVATE_INCLUDES}
	${ENGINE_PUBLIC_INCLUDES}
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

add_subdirectory(../external/spdlog ${CMAKE_CURRENT_BINARY_DIR}/spdlog)
target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE spdlog)

add_subdirectory(../external/glad ${CMAKE_CURRENT_BINARY_DIR}/glad)
target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE glad)

add_subdirectory(../external/glm ${CMAKE_CURRENT_BINARY_DIR}/glm)
target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE glm)

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