##################################
# stereo viewer
##################################

include_directories(${PROJECT_DEPENDENCY_DIRS})

##################################
# Sources

file(GLOB_RECURSE sources_cpp *.cpp)
file(GLOB_RECURSE sources_hpp *.hpp)

include_directories(${PROJECT_SOURCE_DIR}/3rdparty/dataset/include/)
include_directories(${PROJECT_SOURCE_DIR}/3rdparty/utils/include/)
include_directories(${PROJECT_SOURCE_DIR}/modules/stereo_matching/include/)
##################################

##################################
# Target

add_executable(stereo_viewer ${sources_cpp} ${sources_hpp})
target_link_libraries(stereo_viewer utils dataset stereo_matching ${PROJECT_DEPENDENCY_LIBS})
##################################
