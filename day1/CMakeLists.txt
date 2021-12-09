add_executable(part1 part1.cpp)
target_link_libraries(part1 PRIVATE project_options project_warnings range-v3
                                    fmt::fmt-header-only)

add_executable(part2 part2.cpp)
target_link_libraries(part2 PRIVATE project_options project_warnings range-v3
                                    fmt::fmt-header-only)
