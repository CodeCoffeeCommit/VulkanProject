@echo off
echo Compiling GLSL shaders to SPIR-V...

set VULKAN_SDK=C:\VulkanSDK\1.4.328.1
set GLSLC=%VULKAN_SDK%\Bin\glslc.exe

cd /d %~dp0

if not exist compiled mkdir compiled

echo Compiling workbench.vert...
%GLSLC% workbench.vert -o compiled/workbench.vert.spv
if %errorlevel% neq 0 echo FAILED: workbench.vert

echo Compiling workbench.frag...
%GLSLC% workbench.frag -o compiled/workbench.frag.spv
if %errorlevel% neq 0 echo FAILED: workbench.frag

echo Compiling grid.vert...
%GLSLC% grid.vert -o compiled/grid.vert.spv
if %errorlevel% neq 0 echo FAILED: grid.vert

echo Compiling grid.frag...
%GLSLC% grid.frag -o compiled/grid.frag.spv
if %errorlevel% neq 0 echo FAILED: grid.frag

echo.
echo Done!
pause