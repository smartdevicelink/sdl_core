
call "setup_VS2013_x64.cmd"

cd build/sdl_win_x64

call "%programfiles(x86)%\MSBuild\12.0\Bin\MSBuild.exe" smartDeviceLinkCore.sln /t:Build /p:Configuration=Debug

call "%programfiles(x86)%\MSBuild\12.0\Bin\MSBuild.exe" RUN_TESTS.vcxproj /t:Build /p:Configuration=Debug
