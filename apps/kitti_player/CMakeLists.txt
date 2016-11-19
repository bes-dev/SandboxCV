##################################
# kitti player
##################################

include_directories(${PROJECT_DEPENDENCY_DIRS})

##################################
# Sources

file(GLOB_RECURSE sources_cpp *.cpp)
file(GLOB_RECURSE sources_hpp *.hpp)

include_directories(${PROJECT_SOURCE_DIR}/3rdparty/dataset/include/)
include_directories(${PROJECT_SOURCE_DIR}/3rdparty/utils/include/)
##################################

##################################
# Target

add_executable(kitti_player ${sources_cpp} ${sources_hpp})
target_link_libraries(kitti_player dataset utils ${PROJECT_DEPENDENCY_LIBS})
##################################
