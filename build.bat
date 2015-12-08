@echo off

set SRC_DIR=%CD%
for %%i in (.) do set UTIL_NAME=%%~nxi

set OUTPUT_FILE=%UTIL_NAME%.exe
set SRC_FILE="%SRC_DIR%\%UTIL_NAME%.cpp"
set LOCK_FILE=%OUTPUT_FILE%.lock

call buildenv.bat

cl %CFLAGS% %SRC_FILE% /Fe%OUTPUT_FILE% %LDFLAGS%
popd
