add_executable(day24 part1and2.cpp)
# find_package(Boost REQUIRED) target_include_directories(day23part1 PRIVATE
# ${Boost_INCLUDE_DIRS})
target_link_libraries(day24 PRIVATE project_options project_warnings range-v3
                                    fmt::fmt-header-only)

# add_executable(day23part2 part2.cpp) target_include_directories(day23part2
# PRIVATE ${Boost_INCLUDE_DIRS}) target_link_libraries( day23part2 PRIVATE
# project_options project_warnings range-v3 fmt::fmt-header-only
# ${Boost_LIBRARIES})
