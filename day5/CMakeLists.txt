add_executable(day5part1 part1.cpp)
target_link_libraries(day5part1 PRIVATE project_options project_warnings
                                        range-v3 fmt::fmt-header-only)

add_executable(day5part2 part2.cpp)
target_link_libraries(day5part2 PRIVATE project_options project_warnings
                                        range-v3 fmt::fmt-header-only)
