cmake_minimum_required(VERSION 2.6)

PROJECT(Hokuyo)

FIND_PACKAGE(VTK REQUIRED)
INCLUDE(${VTK_USE_FILE})

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