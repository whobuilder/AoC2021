add_executable(day25part1 part1.cpp)
find_package(Boost REQUIRED)
target_include_directories(day25part1 PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(day25part1 PRIVATE project_options project_warnings
                                         range-v3 fmt::fmt-header-only)
