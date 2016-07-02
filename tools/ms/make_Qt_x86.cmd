call "%~dp0\setup_Qt_x86.cmd"

call "%ProgramFiles(x86)%\Microsoft Visual Studio 10.0\VC\vcvarsall.bat" x86

cd %SDL_BUILD_DIR%\sdl_win_qt_x86
call %SDL_QT_DIR%\Tools\QtCreator\bin\jom
call nmake test
