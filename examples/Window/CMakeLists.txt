cmake_minimum_required(VERSION 3.12)
project(Window)
add_executable(Window
	main.cpp
)
target_link_libraries(Window
	firesteel
	glad
	imgui
	glm
)
target_compile_features(Window PUBLIC cxx_std_17)
target_include_directories(Window PUBLIC include)
target_include_directories(Window PUBLIC engine/external/glfw)
set_target_properties(Window PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)