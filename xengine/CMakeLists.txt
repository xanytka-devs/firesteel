cmake_minimum_required(VERSION 3.12)
project(xengine)
set(ENGINE_PROJECT_NAME xengine)

file(GLOB_RECURSE ENGINE_INCLUDES "*.hpp")
file(GLOB_RECURSE ENGINE_SOURCES "*.cpp")
file(GLOB_RECURSE ENGINE_PKG_INCLUDES "../packages/*.hpp" "../packages/*.h")
file(GLOB_RECURSE ENGINE_PKG_SOURCES "../packages/*.cpp")

set(ENGINE_ALL
	${ENGINE_INCLUDES}
	${ENGINE_SOURCES}
	${ENGINE_PKG_INCLUDES}
	${ENGINE_PKG_SOURCES}
)

add_library(${ENGINE_PROJECT_NAME} STATIC
	${ENGINE_ALL}
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/../ FILES
	${ENGINE_ALL}
)
get_filename_component(VULKAN_LIB_PATH ../packages/xe.rendering.vulkan/lib/ ABSOLUTE)
set(CMAKE_VS_SDK_LIBRARY_DIRECTORIES ${VULKAN_LIB_PATH};$(VC_LibraryPath_x64);$(WindowsSDK_LibraryPath_x64))
get_filename_component(VULKAN_LIB_PATH ../packages/xe.rendering.vulkan/lib/vulkan-1.lib ABSOLUTE)
target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE ${VULKAN_LIB_PATH})

get_filename_component(FT_LIB_PATH ../external/freetype/freetype.lib ABSOLUTE)
target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE ${FT_LIB_PATH})

target_include_directories(${ENGINE_PROJECT_NAME} PUBLIC includes)
target_include_directories(${ENGINE_PROJECT_NAME} PUBLIC stb)
target_include_directories(${ENGINE_PROJECT_NAME} PRIVATE src)
target_compile_features(${ENGINE_PROJECT_NAME} PUBLIC cxx_std_17)

add_subdirectory(../external/nvtx/c ${CMAKE_CURRENT_BINARY_DIR}/nvtx)
target_link_libraries(${ENGINE_PROJECT_NAME} PUBLIC nvtx3-cpp)

add_subdirectory(../external/glfw ${CMAKE_CURRENT_BINARY_DIR}/glfw)
target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE glfw)

add_subdirectory(../packages/xe.rendering.opengl/glad ${CMAKE_CURRENT_BINARY_DIR}/glad)
target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE glad)

add_subdirectory(../external/glm ${CMAKE_CURRENT_BINARY_DIR}/glm)
target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE glm)

add_subdirectory(../external/assimp ${CMAKE_CURRENT_BINARY_DIR}/assimp)
target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE assimp)

set(IMGUI_INCLUDES
	../external/imgui/imgui.h
	../external/imgui/misc/cpp/imgui_stdlib.h
	../external/imgui/backends/imgui_impl_glfw.h
	../external/imgui/backends/imgui_impl_opengl3.h
)
set(IMGUI_SOURCES
	../external/imgui/imgui.cpp
	../external/imgui/backends/imgui_impl_glfw.cpp
	../external/imgui/backends/imgui_impl_opengl3.cpp
	../external/imgui/misc/cpp/imgui_stdlib.cpp
	../external/imgui/imgui_draw.cpp
	../external/imgui/imgui_tables.cpp
	../external/imgui/imgui_widgets.cpp
	../external/imgui/imgui_demo.cpp
)
add_library(ImGui STATIC
	${IMGUI_INCLUDES}
	${IMGUI_SOURCES}
)
target_include_directories(ImGui PUBLIC ../external)
target_include_directories(ImGui PUBLIC ../external/imgui)
target_link_libraries(ImGui PRIVATE glad glfw)
target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE ImGui)

set_target_properties(xengine PROPERTIES FOLDER "XEngine")

set_target_properties(assimp PROPERTIES FOLDER "External")
set_target_properties(glad PROPERTIES FOLDER "External")
set_target_properties(glfw PROPERTIES FOLDER "External")
set_target_properties(glm PROPERTIES FOLDER "External")
set_target_properties(ImGui PROPERTIES FOLDER "External")
set_target_properties(UpdateAssimpLibsDebugSymbolsAndDLLs PROPERTIES FOLDER "External")
set_target_properties(zlibstatic PROPERTIES FOLDER "External")