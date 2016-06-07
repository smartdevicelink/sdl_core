@echo off

set SDL_CMAKE_GENERATOR_NAME="Visual Studio 12 Win64"
set SDL_BUILD=sdl_win_x64
set SDL_ROOT=%CD%

call "%ProgramFiles(x86)%\Microsoft Visual Studio 12.0\VC\bin\x86_amd64\vcvarsx86_amd64.bat" x86_amd64

call "%SDL_ROOT%\ms\common_setup.cmd"
if %errorlevel% == 2 exit /b 1

call "%SDL_ROOT%\ms\openssl_setup.cmd" x64

call "%SDL_ROOT%\ms\libusbx_setup.cmd" x64

call "%SDL_ROOT%\ms\cmake_generate.cmd" WinNative_x64