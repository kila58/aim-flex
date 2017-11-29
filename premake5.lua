-- premake5.lua
workspace "aim-flex"
   location "project"
   configurations { "Release" }

project "aim-flex"
   location "project"
   kind "SharedLib"
   language "C++"
   architecture "x86"
   targetdir "bin"
   systemversion "10.0.16299.0"

   files { "src/**.hpp", "src/**.cpp" }

   filter "configurations:Release"
      optimize "On"
