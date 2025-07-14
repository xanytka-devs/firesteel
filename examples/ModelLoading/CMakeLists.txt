cmake_minimum_required(VERSION 3.12)
project(ModelLoading)
add_executable(ModelLoading
	main.cpp
)
target_link_libraries(ModelLoading
	firesteel
	glad
	imgui
	glm
)
target_compile_features(ModelLoading PUBLIC cxx_std_17)
target_include_directories(ModelLoading PUBLIC include)
target_include_directories(ModelLoading PUBLIC engine/external/glfw)
set_target_properties(ModelLoading PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)