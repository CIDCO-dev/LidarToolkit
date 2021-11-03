cmake_minimum_required (VERSION 3.0)
project (CALIBRATION)

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


