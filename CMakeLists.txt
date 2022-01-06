cmake_minimum_required (VERSION 3.0)
project (CALIBRATION)

#C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB SOURCES "src/*.cpp")

add_executable (calibration ${SOURCES})

# Add Eigen
find_package (Eigen3 3.3 REQUIRED)

# Add PCL
find_package(PCL 1.8 REQUIRED COMPONENTS common io sample_consensus)
include_directories(${PCL_INCLUDE_DIRS})
link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})
target_link_libraries(calibration Eigen3::Eigen ${PCL_LIBRARIES})

# Add unit tests
#find_package(Catch2 3 REQUIRED)
#add_executable(tests test/test.cpp)
#target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

add_executable(run_tests test/tests.cpp)
target_include_directories(run_tests PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
set(url https://github.com/philsquared/Catch/releases/download/v2.13.6/catch.hpp)
file(
  DOWNLOAD ${url} "${CMAKE_CURRENT_BINARY_DIR}/catch.hpp"
  STATUS status
  EXPECTED_HASH SHA256=681e7505a50887c9085539e5135794fc8f66d8e5de28eadf13a30978627b0f47)
list(GET status 0 error)
if(error)
  message(FATAL_ERROR "Could not download ${url}")
endif()
target_include_directories(run_tests PUBLIC "${CMAKE_CURRENT_BINARY_DIR}")
