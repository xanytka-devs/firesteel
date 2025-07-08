cmake_minimum_required(VERSION 3.12)
project(ModelLoadingTest)
add_executable(ModelLoadingTest
	main.cpp
)
target_link_libraries(ModelLoadingTest
	firesteel
	glad
	imgui
	glm
)
target_compile_features(ModelLoadingTest PUBLIC cxx_std_17)
target_include_directories(ModelLoadingTest PUBLIC include)
target_include_directories(ModelLoadingTest PUBLIC engine/external/glfw)
set_target_properties(ModelLoadingTest PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)