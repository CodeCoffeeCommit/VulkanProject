@echo off
echo Compiling shaders...

set VULKAN_SDK=C:\VulkanSDK\1.4.328.1
set GLSLC=%VULKAN_SDK%\Bin\glslc.exe

if not exist compiled mkdir compiled

%GLSLC% workbench.vert -o compiled/workbench.vert.spv
%GLSLC% workbench.frag -o compiled/workbench.frag.spv
%GLSLC% grid.vert -o compiled/grid.vert.spv
%GLSLC% grid.frag -o compiled/grid.frag.spv

echo Done!
pause