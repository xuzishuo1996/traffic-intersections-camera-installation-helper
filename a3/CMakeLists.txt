cmake_minimum_required(VERSION 2.8)
project(ece650-a3)

## ECE650 Specific configuration start here
## Project specific configurations go here

# enable c++11 support
set (CMAKE_CXX_FLAGS "-std=c++11 -Wall ${CMAKE_CXX_FLAGS}")
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/run)

# create the main executable
## add additional .cpp files if needed
add_executable(rgen Point.cpp SegUtility.cpp rgen.cpp)
add_executable(ece650-a2 Graph.cpp ece650-a2.cpp)
add_executable(ece650-a3 ece650-a3.cpp)

# create the executable for tests
add_executable(tests test.cpp)
install(PROGRAMS ece650-a1.py command_parser.py graph.py graph_generator.py intersect.py street_db.py DESTINATION bin)
install(TARGETS rgen ece650-a2 ece650-a3 RUNTIME DESTINATION bin)
