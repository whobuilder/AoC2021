add_executable(day12part1 part1.cpp)
target_link_libraries(day12part1 PRIVATE project_options project_warnings
                                         range-v3 fmt::fmt-header-only)

add_executable(day12part2 part2.cpp)
target_link_libraries(day12part2 PRIVATE project_options project_warnings
                                         range-v3 fmt::fmt-header-only)
