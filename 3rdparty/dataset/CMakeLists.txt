##################################
# dataset library
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

add_library(dataset ${sources_cpp} ${sources_hpp})
target_link_libraries(dataset ${PROJECT_DEPENDENCY_LIBS})
##################################
