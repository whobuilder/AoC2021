add_executable(day16part1and2 part1and2.cpp)
target_link_libraries(day16part1and2 PRIVATE project_options project_warnings
                                             range-v3 fmt::fmt-header-only)
