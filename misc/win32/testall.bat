@echo off
setlocal EnableDelayedExpansion
pushd %~dp0\..\..\build
for %%X in ("*.exe") do (
	%%X input%%~nX.txt > tempout.txt
	set RESULT=Fail
	fc tempout.txt output%%~nX.txt >NUL
	if !ERRORLEVEL! == 0 set RESULT=Ok!
	echo puzzle %%~nX: !RESULT!
)
popd
