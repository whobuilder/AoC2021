add_executable(day22part1 part1.cpp)
find_package(Boost REQUIRED)
target_include_directories(day22part1 PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(
  day22part1 PRIVATE project_options project_warnings range-v3
                     fmt::fmt-header-only ${Boost_LIBRARIES})

add_executable(day22part2 part2.cpp)
target_link_libraries(day22part2 PRIVATE project_options project_warnings
                                         range-v3 fmt::fmt-header-only)
