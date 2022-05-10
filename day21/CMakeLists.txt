add_executable(day21part1 part1.cpp)
target_link_libraries(day21part1 PRIVATE project_options project_warnings
                                         range-v3 fmt::fmt-header-only)
