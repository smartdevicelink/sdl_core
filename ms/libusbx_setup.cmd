@echo off
setlocal EnableDelayedExpansion

if [%1]==[] goto usage

set BUILD_TYPE=%1
set LIBS_DIR="%SDL_ROOT%\src\3rd_party-static"
set LIBUSBX_SRC_DIR_NAME=libusbx-1.0.17
set LIBUSBX_SRC_DIR=%LIBS_DIR%\%LIBUSBX_SRC_DIR_NAME%
set LIBUSBX_BUILD_DIR=%BUILD_DIR%\libusbx_win_%BUILD_TYPE%

if not exist %LIBUSBX_SRC_DIR% (
  pushd %LIBS_DIR%
  @echo Cloning libusbx to the %LIBUSBX_SRC_DIR%

  git.exe clone https://github.com/dickens/libusbx-hp.git %LIBUSBX_SRC_DIR_NAME%
  popd
)

if not exist %LIBUSBX_BUILD_DIR% (
  pushd %LIBUSBX_SRC_DIR%\msvc
  call git.exe clean -fxd
  call git.exe checkout 6cba5d96767b205fc653e3273fba81b59f1e1492

  @echo Build libusbx for VC %BUILD_TYPE%
  
  set LIBUSBX_RELEASE_BUILD_PARAMS=/t:Rebuild /p:OutDir=%LIBUSBX_BUILD_DIR%\bin,Configuration=Release
  
  if %BUILD_TYPE%==x86 (
	set LIBUSBX_RELEASE_BUILD_PARAMS=libusb_static_2010.vcxproj !LIBUSBX_RELEASE_BUILD_PARAMS! /p:Platform=Win32
  ) else if %BUILD_TYPE%==x64 (
	set LIBUSBX_RELEASE_BUILD_PARAMS=libusb_static_2012.vcxproj !LIBUSBX_RELEASE_BUILD_PARAMS! /p:Platform=x64,PlatformToolset=v120
  ) else goto usage
  
  call "C:\Program Files (x86)\MSBuild\12.0\Bin\MSBuild.exe" !LIBUSBX_RELEASE_BUILD_PARAMS!
  popd

  xcopy %LIBUSBX_SRC_DIR%\libusb %LIBUSBX_BUILD_DIR%\libusb /E /I /H /R /Y
)

goto end

:usage
@echo "Usage: <script> [x86|x64]"

:end
