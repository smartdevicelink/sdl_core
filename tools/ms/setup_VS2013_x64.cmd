@echo off

rem Next lines extract sdl_core root directory from path
for %%a in (%~dp0\.) do set SDL_ROOT_DIR=%%~dpa
for %%a in (%SDL_ROOT_DIR%\.) do set SDL_ROOT_DIR=%%~dpa
set SDL_ROOT_DIR=%SDL_ROOT_DIR:~0,-1%

set SDL_BUILD_DIR=%SDL_ROOT_DIR%\build

setlocal EnableDelayedExpansion

set SDL_NATIVE_BUILD_DIR=%SDL_BUILD_DIR%\sdl_win_x64

if not exist %SDL_NATIVE_BUILD_DIR% mkdir %SDL_NATIVE_BUILD_DIR%
if not exist %SDL_NATIVE_BUILD_DIR% goto builddir_error

call "%ProgramFiles(x86)%\Microsoft Visual Studio 12.0\VC\bin\x86_amd64\vcvarsx86_amd64.bat" x86_amd64

call "%SDL_ROOT_DIR%\tools\ms\openssl_setup.cmd" x64
call "%SDL_ROOT_DIR%\tools\ms\libusbx_setup.cmd" x64
call "%SDL_ROOT_DIR%\tools\ms\make_sqlite_lib.cmd"

pushd %SDL_NATIVE_BUILD_DIR%
if %errorlevel% == 1 goto builddir_error

set CMAKE_COMMAND=cmake -G "Visual Studio 12 Win64" -DARCH_X64=1 %SDL_ROOT_DIR%
@echo Generating for the win native x64: !CMAKE_COMMAND!
call !CMAKE_COMMAND!

goto end

:builddir_error
@echo ERROR: Cannot access %SDL_NATIVE_BUILD_DIR%
exit /B 1

:end
popd
