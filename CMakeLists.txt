cmake_minimum_required(VERSION 3.15)
project(ASI_Loader)

set(CMAKE_CXX_STANDARD 17)

add_library(ASI_Loader SHARED ASI_Loader.cpp)
target_link_libraries(ASI_Loader shlwapi)

set_target_properties(ASI_Loader PROPERTIES SUFFIX ".asi")
