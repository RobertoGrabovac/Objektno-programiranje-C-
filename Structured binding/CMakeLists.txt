cmake_minimum_required(VERSION 3.11)

project(StructBind LANGUAGES CXX)

enable_testing()

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.11.0
)

# donjih 5 redaka dodano
FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt
  GIT_TAG   10.1.1
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(fmt) #dodano
FetchContent_MakeAvailable(googletest)

find_package(fmt REQUIRED)

add_subdirectory(src)
add_subdirectory(test)
