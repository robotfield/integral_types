# PROJECT_IS_TOP_LEVEL requires version 3.21
cmake_minimum_required(VERSION 3.21) 

project(
  integral_types
  VERSION 0.0.0 
  DESCRIPTION "Like cstdint, but templated on the number of bits"
  LANGUAGES CXX
)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

if(CMAKE_CXX_STANDARD LESS 11)
  message(WARNING "Incompatible C++ standard ${CMAKE_CXX_STANDARD}. ${PROJECT_NAME} requires C++11.")
endif()

# Library target
add_library(
  integral_types INTERFACE 
  src/integral_types/integral_types.hpp 
)
target_include_directories(integral_types INTERFACE src)

# Testing
if((IS_DIRECTORY tests) AND (PROJECT_IS_TOP_LEVEL OR INTEGRAL_TYPES_CMAKE_BUILD_TESTING))
  message("Found '${PROJECT_NAME}' tests, setting those up...")
  include(CTest)
  add_subdirectory(tests)
endif()

