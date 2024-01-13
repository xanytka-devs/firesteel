cmake_minimum_required(VERSION 3.12)
project(glad)

add_library(glad STATIC include/KHR/khrplatform.h include/glad/glad.h src/glad.c)

target_include_directories(glad PUBLIC include)

set_target_properties(glad PROPERTIES FOLDER "External")