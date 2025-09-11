@ECHO OFF
:: Get input
set /P prj_name=App name (no spaces allowed):
set /P gen="Generator (Visual Studio or other):
set /P architecture=Target architecture (x64, x86 or both):
:: Generate application project by CMake
cd ..

echo cmake_minimum_required(VERSION 3.12)^
project(%prj_name%)^
add_subdirectory(engine)^
^
add_executable(%prj_name%^
	"main.cpp"^
)^
target_link_libraries(%prj_name%^
	firesteel^
	glad^
	imgui^
	glm^
)^
target_compile_features(%prj_name% PUBLIC cxx_std_17)^
target_include_directories(%prj_name% PUBLIC includes)^
target_include_directories(%prj_name% PUBLIC engine/external/glfw)^
set_target_properties(%prj_name% PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)>CMakeLists.txt

echo #include "engine/include/app.hpp"^
using namespace Firesteel;^
^
class %prj_name% : public Firesteel::App {^
	// Runs after window and renderer initialization.^
	virtual void onInitialize() override {^
		LOG_INFO("Hello World!");^
	}^
	// Runs each frame.
	virtual void onUpdate() override {}^
	// Runs after window.close() is called or on window closing.
	virtual void onShutdown() override {}^
};^
^
int main() {^
    return %prj_name%{}.start();^
}>main.cpp

mkdir build
cd build
if %gen%==Visual Studio (
	if %architecture%=="x64" (
		cmake -A x64 -S ..
		goto :eof
	)
	if %architecture%==x86 (
		cmake -A Win32 -S ..
		goto :eof
	)
	echo You selected both architectures. Currently building for both of them isn't tested, so do it on your own risk.
	pause
	cmake -A Win32 -B x86 -DFS_BIN_ADDITIONAL="x86/" -S ..
	cmake -A x64 -B x64 -DFS_BIN_ADDITIONAL="x64/" -S ..
	goto :eof
)
if %architecture%=="x64" (
	cmake -A x64 -S ..
	goto :eof
)
if %architecture%=="x86" (
	cmake -A x86 -S ..
	goto :eof
)
echo You selected both architectures. Currently building for both of them isn't tested, so do it on your own risk.
pause
cmake -A x86 -B x86 -DFS_BIN_ADDITIONAL="x86/" -S ..
cmake -A x64 -B x64 -DFS_BIN_ADDITIONAL="x64/" -S ..
:eof