workspace "toy_json"
  configurations { "Debug", "Release" }
  buildoptions "-std=c++11" 		
  location "build"

project "toy_json"
  targetname "toyjson"
  kind  "StaticLib"
  language "C++"

  files {"**.h", "**.cpp"}
  removefiles {"test/**.cpp"}


project "unit_test"
  kind "ConsoleApp"
  language "C++"
  includedirs "lib"
  links {"toy_json", "gtest", "pthread"}

  files {"test/**.cpp"}
