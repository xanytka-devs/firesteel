set /P architecture=Target architecture (x64, x86 or both):
mkdir build
cd build
if %architecture%==x64 (
	cmake -A x64 -S ..
	goto :eof
)
if %architecture%==x86 (
	cmake -A x86 -S ..
	goto :eof
)
echo You selected both architectures. Currently building for both of them isn't tested, so do it on your own risk.
pause
cmake -A x86 -B x86 -DFS_BIN_ADDITIONAL="x86/" -S ..
cmake -A x64 -B x64 -DFS_BIN_ADDITIONAL="x64/" -S ..
:eof