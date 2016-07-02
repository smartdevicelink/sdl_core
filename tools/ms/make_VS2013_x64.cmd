call "%~dp0\setup_VS2013_x64.cmd"

call "%ProgramFiles(x86)%\Microsoft Visual Studio 12.0\VC\bin\x86_amd64\vcvarsx86_amd64.bat" x86_amd64

cd %SDL_BUILD_DIR%\sdl_win_x64
call "C:\Program Files (x86)\MSBuild\12.0\Bin\MSBuild.exe" smartDeviceLinkCore.sln /t:Build /p:Configuration=Debug
call "C:\Program Files (x86)\MSBuild\12.0\Bin\MSBuild.exe" RUN_TESTS.vcxproj /t:Build /p:Configuration=Debug
