add_executable(day15part1 part1.cpp)
target_link_libraries(day15part1 PRIVATE project_options project_warnings
                                         range-v3 fmt::fmt-header-only)

add_executable(day15part2 part2.cpp)
target_link_libraries(day15part2 PRIVATE project_options project_warnings
                                         range-v3 fmt::fmt-header-only)
