@echo off
rem Get project info.
echo XEngine project generator
set /p prj_name=Project name (without spaces): 
rem Ask for CMake usage.
set /p use_cmake=Use CMake (y/n): 
set test=false
if "%use_cmake%" == "y" (set test=true)
if "%use_cmake%" == "Y" (set test=true)
rem Create project dir.
echo Creating project directory...
if not exist "%cd%\%prj_name%\" mkdir "%cd%\%prj_name%"
rem Create src dir.
mkdir "%cd%\%prj_name%\src"
rem Generate main.cpp file.
set cpp_file="%cd%\%prj_name%\src\main.cpp"
echo #include ^<memory^>>>%cpp_file%
echo #include ^<xengine/app.hpp^>>>%cpp_file%
echo.>>%cpp_file%
echo using namespace XEngine;>>%cpp_file%
echo class %prj_name% : public App { };>>%cpp_file%
echo.>>%cpp_file%
echo int main() { >>%cpp_file%
echo     //Create instance of app.>>%cpp_file%
echo     auto app = std::make_unique^<%prj_name%^>();>>%cpp_file%
echo     int returnC = app-^>start(800, 600, "%prj_name%");>>%cpp_file%
echo     return 0;>> %cpp_file%
echo }>>%cpp_file%
rem Check CMake variable.
if "%test%"=="true" (goto gen_cmake)
goto exit
:gen_cmake
rem Generate CMakeLists.txt file.
echo Generate CMakeLists.txt...
set cmake_file="%cd%\%prj_name%\CMakeLists.txt"
echo %cmake_file%
echo cmake_minimum_required(VERSION 3.12)>>%cmake_file%
echo project(%prj_name%)>>%cmake_file%
echo set(%prj_name%_PROJECT_NAME %prj_name%)>>%cmake_file%
echo.>>%cmake_file%
echo set(%prj_name%_SOURCES>>%cmake_file%
echo 	src/main.cpp>>%cmake_file%
echo )>>%cmake_file%
echo.>>%cmake_file%
echo add_executable(${%prj_name%_PROJECT_NAME}>>%cmake_file%
echo 	${%prj_name%_SOURCES}>>%cmake_file%
echo )>>%cmake_file%
echo.>>%cmake_file%
echo target_link_libraries(${%prj_name%_PROJECT_NAME} xengine)>>%cmake_file%
echo target_compile_features(${%prj_name%_PROJECT_NAME} PUBLIC cxx_std_17)>>%cmake_file%
echo target_include_directories(${%prj_name%_PROJECT_NAME} PRIVATE src)>>%cmake_file%
echo.>>%cmake_file%
echo set_target_properties(%prj_name% PROPERTIES FOLDER "XEngine")>>%cmake_file%
rem Add new project to general CMakeLists.txt file.
echo add_subdirectory(%prj_name%) >> CMakeLists.txt
rem Build with CMake.
echo Building...
call AutoCMake.cmd
:exit