##################################
# utils library
##################################

include_directories(${PROJECT_DEPENDENCY_DIRS})

##################################
# Sources

file(GLOB_RECURSE sources_cpp src/*.cpp)
file(GLOB_RECURSE sources_hpp include/*.hpp)

include_directories(include)
##################################

##################################
# Target

add_library(utils ${sources_cpp} ${sources_hpp})
target_link_libraries(utils ${PROJECT_DEPENDENCY_LIBS})
##################################
