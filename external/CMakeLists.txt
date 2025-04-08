cmake_minimum_required(VERSION 3.12)
project(imgui)

set(IMGUI_INCLUDES
	imgui/imgui.h
	imgui/misc/cpp/imgui_stdlib.h
	imgui/backends/imgui_impl_glfw.h
	imgui/backends/imgui_impl_opengl3.h
)
set(IMGUI_SOURCES
	imgui/imgui.cpp
	imgui/backends/imgui_impl_glfw.cpp
	imgui/backends/imgui_impl_opengl3.cpp
	imgui/misc/cpp/imgui_stdlib.cpp
	imgui/imgui_draw.cpp
	imgui/imgui_tables.cpp
	imgui/imgui_widgets.cpp
	imgui/imgui_demo.cpp
)
add_library(imgui STATIC
	${IMGUI_INCLUDES}
	${IMGUI_SOURCES}
)
target_include_directories(imgui PUBLIC ../external)
target_include_directories(imgui PUBLIC ../external/imgui)
target_link_libraries(imgui PRIVATE glad glfw)