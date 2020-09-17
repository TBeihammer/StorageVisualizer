
@echo off
echo "Make sure the environmentvariable QTDIR is set!"
echo %QTDIR%\bin

SET PATH=%PATH%;%QTDIR%\bin

mkdir .\StorageVisualizer
xcopy ".\StorageVisualizer.exe" ".\StorageVisualizer"

windeployqt .\StorageVisualizer\StorageVisualizer.exe

pause