cmake_minimum_required(VERSION 3.12)
project(XEngine)
set(ENGINE_PROJECT_NAME XEngine)

set(ENGINE_PUBLIC_INCLUDES
	includes/stb/stb_image.hpp
	includes/XEngine/ResManager.hpp
	includes/XEngine/App.hpp
)
set(ENGINE_PRIVATE_INCLUDES
	includes/XEngine/Log.hpp
	includes/XEngine/Rendering/Shader.hpp
)
set(ENGINE_PRIVATE_SOURCES
	src/XEngine/ResManager.cpp
	src/XEngine/App.cpp
	src/XEngine/Log.cpp
	src/XEngine/Rendering/Shader.cpp
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