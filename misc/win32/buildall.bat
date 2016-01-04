@echo off
pushd %~dp0..\..\src
for %%f in ("*.cpp") do (
	call build %%~nf %1
)
popd
