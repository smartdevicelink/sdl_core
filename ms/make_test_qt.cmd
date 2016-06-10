
call "setup_Qt_x86.cmd"

cd build/sdl_win_qt_x86
call %SDL_QT_DIR%\Tools\QtCreator\bin\jom
call nmake test