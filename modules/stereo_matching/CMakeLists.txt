##################################
# stereo matching library
##################################

include_directories(${PROJECT_DEPENDENCY_DIRS})

##################################
# Sources

file(GLOB_RECURSE source_cpp src/*.cpp)
file(GLOB_RECURSE source_hpp include/*.hpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/3rdparty/utils/include/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
##################################

##################################
# Target

add_library(stereo_matching ${source_cpp} ${source_hpp})
target_link_libraries(stereo_matching utils ${PROJECT_DEPENDENCY_LIBS})

##################################
