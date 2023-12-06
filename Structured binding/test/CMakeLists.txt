add_executable(utest utest.cpp ../src/log.cpp ../src/log.h)

set_target_properties(utest
    PROPERTIES
    CXX_STANDARD 17
    CXX_EXTENSIONS OFF
    CXX_STANDARD_REQUIRED ON
  )


target_link_libraries(utest gtest_main fmt::fmt-header-only)
add_test(NAME utest COMMAND utest)
