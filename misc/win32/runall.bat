@echo off
pushd %~dp0..\..\build
for %%X in ("*.exe") do (
	echo puzzle %%~nX:
	%%X input%%~nX.txt %*
	echo.
)
popd
