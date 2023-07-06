cmake_minimum_required(VERSION 3.12)
project(XEngineCore)
set(ENGINE_PROJECT_NAME XEngineCore)

add_library(${ENGINE_PROJECT_NAME} STATIC
	src/XEngineCore/Utils/test.cpp
	includes/XEngineCore/Utils/test.hpp
)

target_include_directories(${ENGINE_PROJECT_NAME} PUBLIC includes)
target_compile_features(${ENGINE_PROJECT_NAME} PUBLIC cxx_std_17)