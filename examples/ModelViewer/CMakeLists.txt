cmake_minimum_required(VERSION 3.12)
project(ModelViewer)
add_executable(ModelViewer
	main.cpp
)
target_link_libraries(ModelViewer
	firesteel
	glad
	imgui
	glm
)
target_compile_features(ModelViewer PUBLIC cxx_std_17)
target_include_directories(ModelViewer PUBLIC include)
target_include_directories(ModelViewer PUBLIC engine/external/glfw)
set_target_properties(ModelViewer PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)