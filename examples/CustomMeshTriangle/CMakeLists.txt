cmake_minimum_required(VERSION 3.12)
project(CustomMeshTriangle)
add_executable(CustomMeshTriangle
	main.cpp
)
target_link_libraries(CustomMeshTriangle
	firesteel
	glad
	imgui
	glm
)
target_compile_features(CustomMeshTriangle PUBLIC cxx_std_17)
target_include_directories(CustomMeshTriangle PUBLIC include)
target_include_directories(CustomMeshTriangle PUBLIC engine/external/glfw)
set_target_properties(CustomMeshTriangle PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)