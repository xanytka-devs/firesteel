cmake_minimum_required(VERSION 3.12)
project(imgui)

option(IMGUI_BACKEND_OPENGL "Support for rendering in OpenGL" ON)
option(IMGUI_STD "Support for std::string" ON)

if(IMGUI_BACKEND_OPENGL)
	set(IMGUI_BACKEND ${IMGUI_BACKEND}
		imgui/backends/imgui_impl_glfw.h
		imgui/backends/imgui_impl_opengl3.h
		imgui/backends/imgui_impl_glfw.cpp
		imgui/backends/imgui_impl_opengl3.cpp
	)
endif()
if(IMGUI_STD)
	set(IMGUI_ADDITIONAL ${IMGUI_ADDITIONAL}
		imgui/misc/cpp/imgui_stdlib.h
		imgui/misc/cpp/imgui_stdlib.cpp
	)
endif()

set(IMGUI_INCLUDES
	imgui/imgui.h
)
set(IMGUI_SOURCES
	imgui/imgui.cpp
	imgui/imgui_draw.cpp
	imgui/imgui_tables.cpp
	imgui/imgui_widgets.cpp
	imgui/imgui_demo.cpp
)
add_library(imgui STATIC
	${IMGUI_INCLUDES}
	${IMGUI_SOURCES}
	${IMGUI_BACKEND}
	${IMGUI_ADDITIONAL}
)
target_include_directories(imgui PUBLIC ../external/imgui)
target_link_libraries(imgui PRIVATE glad glfw)