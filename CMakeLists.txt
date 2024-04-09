cmake_minimum_required(VERSION 3.10)

project(PirServer VERSION 1.0)


# Globbing source files
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.c" "modules/*.cpp" "modules/*.c")

 # Include directory for libpq headers

add_executable(pirserver 
${SOURCES}
)

target_link_libraries(pirserver PUBLIC)  

add_custom_target(run 
    COMMAND cmake ..
    COMMAND make
    COMMAND pirserver
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Compile and run server"
)

add_custom_target(bundle
    COMMAND cmake ..
    COMMAND make
    COMMAND mkdir -p ../product
    COMMAND cp pirserver ../product
    COMMAND cp -r ../web-views ../product
    
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Build and bundle product distribution"
)