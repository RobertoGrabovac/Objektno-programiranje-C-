cmake_minimum_required(VERSION 3.13)
project(Ranges LANGUAGES CXX)

if(WIN32)
# Pod MSVC nam treba /std:c++latest što se dobiva s 23
set(CMAKE_CXX_STANDARD 23)
# Na Windowsima je fmt instaliran pomoću vcpkg. Ništa nije 
# funkcioniralo do zadavanje pune staze do fmt-config.cmake datoteke.
#set(fmt_DIR "C:\\src\\vcpkg\\packages\\fmt_x64-windows\\share\\fmt")
#set(range-v3_DIR "C:\\src\\vcpkg\\packages\\range-v3_x64-windows\\share\\range-v3")
else()
# Linux još ne pozna -std=c++23
set(CMAKE_CXX_STANDARD 20)
endif()
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#find_package(fmt CONFIG REQUIRED)
#find_package(range-v3 CONFIG REQUIRED)

include(FetchContent)
FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt
  GIT_TAG   9.1.0
)
FetchContent_MakeAvailable(fmt)

FetchContent_Declare(
	Range-v3
  GIT_REPOSITORY https://github.com/ericniebler/range-v3
  GIT_TAG   0.12.0
)
FetchContent_MakeAvailable(fmt)
FetchContent_MakeAvailable(Range-v3)


add_executable(main main.cpp)
target_link_libraries(main PRIVATE range-v3 fmt::fmt-header-only)
