cmake_minimum_required(VERSION 3.12)
project(TriangleTest)
add_executable(TriangleTest
	main.cpp
)
target_link_libraries(TriangleTest
	firesteel
	glad
	imgui
	glm
)
target_compile_features(TriangleTest PUBLIC cxx_std_17)
target_include_directories(TriangleTest PUBLIC include)
target_include_directories(TriangleTest PUBLIC engine/external/glfw)
set_target_properties(TriangleTest PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)