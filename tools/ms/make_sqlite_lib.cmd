@echo off

pushd %SDL_SQLITE_BIN_DIR%
lib /def:sqlite3.def /machine:x64
popd
