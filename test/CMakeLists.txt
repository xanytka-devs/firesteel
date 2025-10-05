cmake_minimum_required(VERSION 3.12)
project(fs_window_test)

add_executable(fs_window_test
	main.cpp
)
target_link_libraries(fs_window_test
	firesteel
	glad
	imgui
	glm
)
target_compile_features(fs_window_test PUBLIC cxx_std_17)
target_include_directories(fs_window_test PUBLIC includes)
target_include_directories(fs_window_test PUBLIC engine/external/glfw)
set_target_properties(fs_window_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)