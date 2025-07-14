cmake_minimum_required(VERSION 3.12)
project(CustomMesh)
add_executable(CustomMesh
	main.cpp
)
target_link_libraries(CustomMesh
	firesteel
	glad
	imgui
	glm
)
target_compile_features(CustomMesh PUBLIC cxx_std_17)
target_include_directories(CustomMesh PUBLIC include)
target_include_directories(CustomMesh PUBLIC engine/external/glfw)
set_target_properties(CustomMesh PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)