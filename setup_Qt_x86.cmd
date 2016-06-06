@echo off

if not defined SDL_QT_DIR (
@echo "ERROR: Set SDL_QT_DIR first. E.g. set SDL_QT_DIR=C:\Qt"
exit /b 1
)

call "%ProgramFiles(x86)%\Microsoft Visual Studio 10.0\VC\vcvarsall.bat" x86

rem "CodeBlocks" is used to generate .cbp file. Otherwise the Qt Creator will popup the wizard to run cmake once more time to generate it.
set SDL_CMAKE_GENERATOR_NAME="CodeBlocks - NMake Makefiles"
set SDL_BUILD=sdl_win_qt_x86
set SDL_ROOT=%CD%


call "%SDL_ROOT%\ms\common_setup.cmd"
if %errorlevel% == 2 exit /b 1

call "%SDL_ROOT%\ms\openssl_setup.cmd" x86

call "%SDL_ROOT%\ms\libusbx_setup.cmd" x86

call "%SDL_ROOT%\ms\cmake_generate.cmd" WinQt_x86