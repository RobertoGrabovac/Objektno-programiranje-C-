add_executable(structbind main.cpp log.cpp log.h)

set_target_properties(structbind 
    PROPERTIES
    CXX_STANDARD 17
    CXX_EXTENSIONS OFF
    CXX_STANDARD_REQUIRED ON
  )


target_link_libraries(structbind fmt::fmt-header-only)
