@echo off
pushd %~dp0\..\..
for /d %%D in ("0*", "1*", "2*") do (
	pushd %%D
	call build.bat
	popd
)
popd
