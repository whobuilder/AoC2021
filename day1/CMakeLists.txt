add_executable(day1part1 part1.cpp)
target_link_libraries(day1part1 PRIVATE project_options project_warnings
                                        range-v3 fmt::fmt-header-only)

add_executable(day1part2 part2.cpp)
target_link_libraries(day1part2 PRIVATE project_options project_warnings
                                        range-v3 fmt::fmt-header-only)
