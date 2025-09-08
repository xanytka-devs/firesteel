cmake_minimum_required(VERSION 3.12)
project(CustomMeshQuad)
add_executable(CustomMeshQuad
	main.cpp
)
target_link_libraries(CustomMeshQuad
	firesteel
	glad
	imgui
	glm
)
target_compile_features(CustomMeshQuad PUBLIC cxx_std_17)
target_include_directories(CustomMeshQuad PUBLIC include)
target_include_directories(CustomMeshQuad PUBLIC engine/external/glfw)
set_target_properties(CustomMeshQuad PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)