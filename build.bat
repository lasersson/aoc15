@echo off

set SRC_DIR=%CD%
set OUTPUT_DIR=..\build
for %%i in (.) do set UTIL_NAME=%%~nxi

set OUTPUT_FILE=%UTIL_NAME%.exe
set SRC_FILE="%SRC_DIR%\%UTIL_NAME%.cpp"

set CFLAGS_COMMON=/nologo /fp:fast /Gm- /GR- /EHa- /Oi /WX /W4 /FC /Z7 /I..\include -wd4100 -wd4127 -wd4189 -wd4324 -wd4505 -wd4201 /D_CRT_SECURE_NO_WARNINGS
set CFLAGS_DEBUG=/Od /DAOC_DEBUG %CFLAGS_COMMON%
set CFLAGS_RELEASE=/O2 /DAOC_DEBUG %CFLAGS_COMMON%
set CFLAGS=%CFLAGS_DEBUG%
REM set CFLAGS=%CFLAGS_RELEASE%

set LDFLAGS=/link /incremental:no /opt:ref

if not exist %OUTPUT_DIR% mkdir %OUTPUT_DIR%

pushd %OUTPUT_DIR%
cl %CFLAGS% %SRC_FILE% /Fe%OUTPUT_FILE% %LDFLAGS%
popd
