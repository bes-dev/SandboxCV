#############################################
# cv project
#############################################
cmake_minimum_required(VERSION 2.8)

project(SandboxCV)
set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 1)

#############################################
# Dependencies

#OpenCV
find_package(OpenCV REQUIRED)
if(NOT OpenCV_FOUND)
    message(SEND_ERROR "Faild to find Opencv")
    return()
else()
    set(PROJECT_DEPENDENCY_DIRS ${OpenCV_INCLUDE_DIRS} ${PROJECT_DEPENDENCY_DIRS})
    set(PROJECT_DEPENDENCY_LIBS ${OpenCV_LIBS} ${PROJECT_DEPENDENCY_LIBS})
endif()
#############################################

#############################################
# Definitions

if(WITH_LOG)
  add_definitions(-DENABLE_LOG)
endif()

if(WITH_PERF_REPORT)
  add_definitions(-DENABLE_PERF_REPORT)
endif()
#############################################

#############################################
#General compiler flags

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -mtune=corei7 -march=corei7 -lpthread -O3 -msse4.2")

if (NOT CMAKE_BUILD_TYPE)
  message(STATUS "No build type selected, default to Release")
  set(CMAKE_BUILD_TYPE "Release")
endif()
#############################################

message(STATUS "====== Configuration Summary ======")
message(STATUS "")
message(STATUS "Build type         :    ${CMAKE_BUILD_TYPE}")
message(STATUS "OpenCV_DIR         :    ${OpenCV_DIR}")
message(STATUS "CMAKE_CXX_FLAGS    :    ${CMAKE_CXX_FLAGS}")
message(STATUS "Performance report :    ${WITH_PERF_REPORT}")
message(STATUS "Logging mode       :    ${WITH_LOG}")
message(STATUS "")

add_subdirectory(3rdparty)
add_subdirectory(modules)
add_subdirectory(apps)
