@echo off
setlocal EnableDelayedExpansion
if [%1]==[] goto usage
set BUILD_TYPE=%1
if exist "%SDL_OPENSSL_DIR%\openssl_win_%BUILD_TYPE%"  goto msg
set LIBS_DIR="%SDL_ROOT_DIR%\src\3rd_party-static"
set OPENSSL_SRC_DIR_NAME=openssl-1.0.2
set OPENSSL_SRC_DIR=%LIBS_DIR%\%OPENSSL_SRC_DIR_NAME%
set OPENSSL_BUILD_DIR=%SDL_OPENSSL_DIR%\openssl_win_%BUILD_TYPE%

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
rem The following commit is stable
  call git.exe checkout 688c10544d2ba32428830d0634e91233c20920c1

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
setx SDL_OPENSSL_DIR %OPENSSL_BUILD_DIR%
goto end

:usage
@echo "Usage: <script> [x86|x64]"
:msg
@echo "OpenSSL already exists"
:end
