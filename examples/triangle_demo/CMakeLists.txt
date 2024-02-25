cmake_minimum_required(VERSION 3.12)
project(triangle_demo)
set(TR_PROJECT_NAME triangle_demo)

set(TR_SOURCES
	src/main.cpp
)

add_executable(${TR_PROJECT_NAME}
	${TR_SOURCES}
)

target_link_libraries(${TR_PROJECT_NAME} xengine ImGui glm)
target_compile_features(${TR_PROJECT_NAME} PUBLIC cxx_std_17)
target_include_directories(${TR_PROJECT_NAME} PRIVATE src)

set_target_properties(triangle_demo PROPERTIES FOLDER "XEngine")