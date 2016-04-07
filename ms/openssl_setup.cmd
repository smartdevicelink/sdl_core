@echo off
setlocal EnableDelayedExpansion

if [%1]==[] goto usage

set BUILD_TYPE=%1
set LIBS_DIR="%SDL_ROOT%\src\3rd_party-static"
set OPENSSL_SRC_DIR_NAME=openssl-1.0.2
set OPENSSL_SRC_DIR=%LIBS_DIR%\%OPENSSL_SRC_DIR_NAME%
set OPENSSL_BUILD_DIR=%BUILD_DIR%\openssl_win_%BUILD_TYPE%

if not exist %OPENSSL_SRC_DIR% (
  pushd %LIBS_DIR%
  @echo Cloning OpenSSL to the %OPENSSL_SRC_DIR%

  git.exe clone https://github.com/openssl/openssl.git %OPENSSL_SRC_DIR_NAME%

  popd
)

if not exist %OPENSSL_BUILD_DIR% (

  if %BUILD_TYPE%==x86 (
    set OPENSSL_CONFIG_OPT=VC-WIN32 no-asm no-shared
  ) else if %BUILD_TYPE%==x64 (
    set OPENSSL_CONFIG_OPT=VC-WIN64A no-shared
  ) else goto usage

  pushd %OPENSSL_SRC_DIR%
  call git.exe clean -fxd
  call git.exe checkout origin/OpenSSL_1_0_2-stable

  set OPENSSL_CONFIG_CMD=perl.exe Configure !OPENSSL_CONFIG_OPT! --prefix=!OPENSSL_BUILD_DIR!
  @echo Build OpenSSL for VC %BUILD_TYPE%: !OPENSSL_CONFIG_CMD!

  call !OPENSSL_CONFIG_CMD!

  if %BUILD_TYPE%==x86 (
    call ms\do_ms
  ) else if %BUILD_TYPE%==x64 (
    call ms\do_win64a
  ) else goto usage

  nmake -f ms\nt.mak
  nmake -f ms\nt.mak install

  popd
)

goto end

:usage
@echo "Usage: <script> [x86|x64]"

:end