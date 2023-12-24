cmake_minimum_required(VERSION 3.12)
project(XEngineEditor)
set(EDITOR_PROJECT_NAME XEngineEditor)

#set(EDITOR_INCLUDES
#)
set(EDITOR_SOURCES
	src/main.cpp
)

set(EDITOR_ALL
	${EDITOR_SOURCES}
	#${EDITOR_INCLUDES}
)

add_executable(${EDITOR_PROJECT_NAME}
	${EDITOR_ALL}
)

target_link_libraries(${EDITOR_PROJECT_NAME} XEngine ImGui glm assimp)
target_compile_features(${EDITOR_PROJECT_NAME} PUBLIC cxx_std_17)
target_include_directories(${EDITOR_PROJECT_NAME} PUBLIC includes)
target_include_directories(${EDITOR_PROJECT_NAME} PRIVATE src)

set_target_properties(${EDITOR_PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)