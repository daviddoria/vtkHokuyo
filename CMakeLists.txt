cmake_minimum_required(VERSION 2.6)

PROJECT(Hokuyo)

FIND_PACKAGE(VTK REQUIRED)
INCLUDE(${VTK_USE_FILE})

#INCLUDE_DIRECTORIES(/usr/local/include/urg)
#/usr/local/lib
get_property(inc_dirs DIRECTORY PROPERTY INCLUDE_DIRECTORIES)
message("inc_dirs = ${inc_dirs}")

FIND_PACKAGE(VTK REQUIRED)
INCLUDE(${VTK_USE_FILE})

ADD_EXECUTABLE(Hokuyo Hokuyo.cpp vtkHokuyo.cxx)
TARGET_LINK_LIBRARIES(Hokuyo
urg_monitor
urg
urg_coordinate
urg_geometry
urg_connection
urg_connection_sdl
urg_common
urg_system
SDL
pthread
vtkHybrid
)

#ADD_EXECUTABLE(SingleScan SingleScan.cpp)
#TARGET_LINK_LIBRARIES(SingleScan urg_monitor urg  urg_coordinate urg_geometry urg_connection urg_connection_sdl urg_common urg_system SDL pthread vtkHybrid)

#ADD_EXECUTABLE(Debugging Debugging.cpp)
#TARGET_LINK_LIBRARIES(Debugging vtkHybrid )
