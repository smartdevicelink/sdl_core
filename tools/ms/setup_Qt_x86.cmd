@echo off

rem Next lines extract sdl_core root directory from path
for %%a in (%~dp0\.) do set SDL_ROOT_DIR=%%~dpa
for %%a in (%SDL_ROOT_DIR%\.) do set SDL_ROOT_DIR=%%~dpa
set SDL_ROOT_DIR=%SDL_ROOT_DIR:~0,-1%

set SDL_BUILD_DIR=%SDL_ROOT_DIR%\build

setlocal EnableDelayedExpansion

if not defined SDL_QT_DIR goto qtdir_error

set SDL_QT_BUILD_DIR=%SDL_BUILD_DIR%\sdl_win_qt_x86

if not exist %SDL_QT_BUILD_DIR% mkdir %SDL_QT_BUILD_DIR%
if not exist %SDL_QT_BUILD_DIR% goto builddir_error

call "%ProgramFiles(x86)%\Microsoft Visual Studio 10.0\VC\vcvarsall.bat" x86

call "%SDL_ROOT_DIR%\tools\ms\openssl_setup.cmd" x86
call "%SDL_ROOT_DIR%\tools\ms\libusbx_setup.cmd" x86

pushd %SDL_QT_BUILD_DIR%
if %errorlevel% == 1 goto builddir_error

rem "CodeBlocks" is used to generate .cbp file. Otherwise the Qt Creator will popup the wizard to run cmake once more time to generate it.
set CMAKE_COMMAND=cmake -G "CodeBlocks - NMake Makefiles" -DCMAKE_INSTALL_PREFIX:PATH="%SDL_QT_DIR%\5.5\msvc2010" -DARCH_X86=1 -DSDK_QT=1 -DCMAKE_BUILD_TYPE=Debug %SDL_ROOT_DIR%
@echo Generating for the win Qt x86: !CMAKE_COMMAND!
call !CMAKE_COMMAND!

goto end

:builddir_error
@echo ERROR: Cannot access %SDL_QT_BUILD_DIR%
exit /B 1

:qtdir_error
@echo ERROR: SDL_QT_DIR not found
exit /B 2

:end
popd
