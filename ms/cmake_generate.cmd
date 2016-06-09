@echo off
setlocal EnableDelayedExpansion

if [%1]==[] goto usage
set GEN_TYPE=%1

pushd %SDL_BUILD_DIR%
if %errorlevel% == 1 goto builddir_error

if %GEN_TYPE% == WinQt_x86 (
  set CMAKE_COMMAND=cmake -G %SDL_CMAKE_GENERATOR_NAME% -DCMAKE_INSTALL_PREFIX:PATH="%SDL_QT_DIR%\5.5\msvc2010" -DQT_PORT=1 -DARCH_X86=1 -DSDK_QT=1 -DCMAKE_BUILD_TYPE=Debug %SDL_ROOT%
  @echo Generating for the win Qt x86: !CMAKE_COMMAND!
  call !CMAKE_COMMAND!
  goto end
)
if %GEN_TYPE% == WinNative_x64 (
  set CMAKE_COMMAND=cmake -G %SDL_CMAKE_GENERATOR_NAME% -DARCH_X64=1 %SDL_ROOT%
  @echo Generating for the win native x64: !CMAKE_COMMAND!
  call !CMAKE_COMMAND!
  goto end
)
goto usage

:builddir_error
@echo ERROR: Could not change to %SDL_BUILD_DIR%
exit /b 1

:usage
@echo "Usage: <script> [WinNative_x64|WinQt_x86]"

:end
popd
