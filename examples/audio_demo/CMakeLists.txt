cmake_minimum_required(VERSION 3.12)
project(audio_demo)
set(AUDIO_PROJECT_NAME audio_demo)

set(TR_SOURCES
	src/main.cpp
)

add_executable(${AUDIO_PROJECT_NAME}
	${TR_SOURCES}
)

target_link_libraries(${AUDIO_PROJECT_NAME} xengine ImGui glm)
target_compile_features(${AUDIO_PROJECT_NAME} PUBLIC cxx_std_17)
target_include_directories(${AUDIO_PROJECT_NAME} PRIVATE src)

set_target_properties(audio_demo PROPERTIES FOLDER "XEngine")