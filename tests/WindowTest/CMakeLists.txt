cmake_minimum_required(VERSION 3.12)
project(WindowTest)
add_executable(WindowTest
	main.cpp
)
target_link_libraries(WindowTest
	firesteel
	glad
	imgui
	glm
)
target_compile_features(WindowTest PUBLIC cxx_std_17)
target_include_directories(WindowTest PUBLIC include)
target_include_directories(WindowTest PUBLIC engine/external/glfw)
set_target_properties(WindowTest PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)