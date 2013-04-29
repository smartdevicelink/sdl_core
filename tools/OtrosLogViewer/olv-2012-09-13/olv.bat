echo off
REM *******************************************************************************
REM  Copyright 2011 Krzysztof Otrebski
REM  
REM  Licensed under the Apache License, Version 2.0 (the "License");
REM  you may not use this file except in compliance with the License.
REM  You may obtain a copy of the License at
REM  
REM    http://www.apache.org/licenses/LICENSE-2.0
REM  
REM  Unless required by applicable law or agreed to in writing, software
REM  distributed under the License is distributed on an "AS IS" BASIS,
REM  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
REM  See the License for the specific language governing permissions and
REM  limitations under the License.
REM ******************************************************************************

cd %~dp0%
set MEMORY=-Xmx1024m
set LOG_PROPERTIES=-Djava.util.logging.config.file=logging.properties
rem set SFTP_KEY=-Dvfs.Identities=
if "%1"=="-batch" goto batchMode

if exist "%JAVA_HOME%\bin\javaw.exe" (
  set LOCAL_JAVA="%JAVA_HOME%\bin\javaw.exe"
) else (
  set LOCAL_JAVA=javaw.exe
)
start "OtrosLogViewer" /B %LOCAL_JAVA% %LOG_PROPERTIES% %MEMORY% %SFTP_KEY% -jar lib\OtrosStarter.jar %* 1>olv.out.txt 2>olv.err.txt
goto finish

:batchMode
if exist "%JAVA_HOME%\bin\java.exe" (
  set LOCAL_JAVA="%JAVA_HOME%\bin\java.exe"
) else (
  set LOCAL_JAVA=java.exe
)
echo on
%LOCAL_JAVA% %LOG_PROPERTIES% %MEMORY% %SFTP_KEY% -jar lib\OtrosStarter.jar %*


:finish