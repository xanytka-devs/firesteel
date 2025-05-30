cmake_minimum_required(VERSION 3.12)
project(firesteel)
set(PRJ_NAME firesteel)

message(CHECK_START "Firesteel: Version 0.2.1")

# Fixing library-depndend windows stuff.
set(CMAKE_CXX_FLAGS_RELEASE "/MT /O2 /Ob2 /DNDEBUG" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS_DEBUG "/MTd /Zi /Ob0 /Od /RTC1" CACHE STRING "" FORCE)
set(CMAKE_C_FLAGS_RELEASE "/MT /O2 /Ob2 /DNDEBUG" CACHE STRING "" FORCE)
set(CMAKE_C_FLAGS_DEBUG "/MTd /Zi /Ob0 /Od /RTC1" CACHE STRING "" FORCE)
option(USE_MSVC_RUNTIME_LIBRARY_DLL OFF)
option(GLFW_BUILD_DOCS OFF)
option(GLM_ENABLE_CXX_98 "" ON)
option(GLM_ENABLE_CXX_11 "" ON)
option(GLM_ENABLE_CXX_14 "" ON)
option(GLM_ENABLE_CXX_17 "" ON)
option(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF)
option(ASSIMP_BUILD_TESTS OFF)

option(FS_INCLUDE_ASSIMP "Include Assimp libraries (GRAPHICS)" ON)
option(FS_INCLUDE_GLM "Include GLM library (UI)" ON)
option(FS_INCLUDE_FT "Include FreeType library (UI)" ON)
option(FS_INCLUDE_IMGUI "Include ImGui library (UI)" ON)
option(FS_INCLUDE_OGL "Include OpenGL libraries (GRAPHICS)" ON)
option(FS_PKGS "Include&build external packages in local pkgs folder" ON)
option(FS_PKGS_ALLOW_OVERRIDES "Allows packages to tinker with CMakeLists.txt \
(Be cautious, this can be dangerous for integrity of the library!)" ON)
option(FS_DISABLE_SUBMODULES "Build for CodeQL (Doesn't build modules)" OFF)

MACRO(SUBDIRLIST result curdir)
  FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
  SET(dirlist "")
  FOREACH(child ${children})
    IF(IS_DIRECTORY ${curdir}/${child})
      LIST(APPEND dirlist ${child})
    ENDIF()
  ENDFOREACH()
  SET(${result} ${dirlist})
ENDMACRO()

file(GLOB_RECURSE ENGINE_INCLUDES "include/*.hpp")
file(GLOB_RECURSE ENGINE_SOURCES "include/*.cpp")
if(NOT FS_DISABLE_SUBMODULES)
	if(FS_INCLUDE_FT)
		file(GLOB_RECURSE FT_INCLUDES "external/freetype/*.hpp external/freetype/*.h")
	endif()
else()
	message("Currently build of submodules is disabled. This means library won't be properly built.")
endif()

set(PRJ_ALL
	${ENGINE_INCLUDES}
	${ENGINE_SOURCES}
	${FT_INCLUDES}
	${OCULUS_INCLUDES}
	${LUA_INCLUDES}
)

add_library(${PRJ_NAME}
	${PRJ_ALL}
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/ FILES
	${PRJ_ALL}
)

target_include_directories(${PRJ_NAME} PUBLIC includes)
target_include_directories(${PRJ_NAME} PUBLIC stb)
target_include_directories(${PRJ_NAME} PRIVATE src)
target_compile_features(${PRJ_NAME} PUBLIC cxx_std_17)
get_filename_component(PKGS_DIR
                       "${CMAKE_CURRENT_LIST_DIR}/../pkgs"
                       ABSOLUTE)
if(FS_PKGS AND EXISTS ${PKGS_DIR} AND IS_DIRECTORY ${PKGS_DIR})
	message(CHECK_START "Firesteel: Packages are enabled. Enumerating...")
	target_include_directories(${PRJ_NAME} PUBLIC ${PKGS_DIR})
	if(FS_PKGS_ALLOW_OVERRIDES)
		cmake_policy(SET CMP0079 NEW)
		SUBDIRLIST(SUBDIRS ${PKGS_DIR})
		FOREACH(subdir ${SUBDIRS})
			if(EXISTS "${PKGS_DIR}/${subdir}/CMakeLists.txt")
				message(CHECK_START "Firesteel: Found CMake overrides in pkgs/${subdir}")
				add_subdirectory(${PKGS_DIR}/${subdir} ${CMAKE_CURRENT_BINARY_DIR}/pkgs/${subdir})
			else()
				message(CHECK_START "Firesteel: No CMake overrides found in pkgs/${subdir}")
			endif()
		ENDFOREACH()
	endif()
	message(CHECK_START "Firesteel: Finished enumerating through files.")
else()
	message("Firesteel: Either package enumeration is disabled or '../pkgs' folder doesn't exist. Packages won't be built")
endif()

if(NOT FS_DISABLE_SUBMODULES)
	add_subdirectory(external/glfw ${CMAKE_CURRENT_BINARY_DIR}/glfw)
	target_link_libraries(${PRJ_NAME} PRIVATE glfw)
	set_target_properties(glfw PROPERTIES FOLDER "External/GLFW3")
	set_target_properties(uninstall PROPERTIES FOLDER "External/GLFW3")
	set_target_properties(update_mappings PROPERTIES FOLDER "External/GLFW3")
	target_include_directories(${PRJ_NAME} PUBLIC external/glfw)

	if(FS_INCLUDE_OGL)
		add_subdirectory(external/glad ${CMAKE_CURRENT_BINARY_DIR}/glad)
		target_link_libraries(${PRJ_NAME} PRIVATE glad)
		set_target_properties(glad PROPERTIES FOLDER "External")
	endif()
	
	if(FS_INCLUDE_GLM)
		add_subdirectory(external/glm ${CMAKE_CURRENT_BINARY_DIR}/glm)
		target_link_libraries(${PRJ_NAME} PRIVATE glm)
		set_target_properties(glm PROPERTIES FOLDER "External")
	endif()
	
	if(FS_INCLUDE_ASSIMP)
		add_subdirectory(external/assimp ${CMAKE_CURRENT_BINARY_DIR}/assimp)
		target_link_libraries(${PRJ_NAME} PRIVATE assimp)
		set_target_properties(assimp PROPERTIES FOLDER "External/assimp")
		set_target_properties(UpdateAssimpLibsDebugSymbolsAndDLLs PROPERTIES FOLDER "External/assimp")
		set_target_properties(zlibstatic PROPERTIES FOLDER "External/assimp")
	endif()
	
	if(FS_INCLUDE_FT)
		target_include_directories(${PRJ_NAME} PRIVATE "external\\freetype\\include" "external\\freetype\\")
		target_link_libraries(${PRJ_NAME} PUBLIC "engine\\freetype\\freetype")
	endif()
	
	if(FS_INCLUDE_IMGUI)
		add_subdirectory(external ${CMAKE_CURRENT_BINARY_DIR}/imgui)
		target_include_directories(imgui PUBLIC external/imgui)
		set_target_properties(imgui PROPERTIES FOLDER "External")

		target_include_directories(${PRJ_NAME} PUBLIC external/imgui)
	endif()
endif()