cmake_minimum_required(VERSION 3.10)

project(PirServer VERSION 1.0)


# file(GLOB MODELS ./models/*.cpp)
# file(GLOB CONTROLLERS ./controllers/*.cpp)
 # Include directory for libpq headers

add_executable(main 
main.cpp 
controllers/Server.cpp
controllers/Controller.cpp 
controllers/Filter.cpp 
models/PIRDB.cpp 
models/Request.cpp 
models/Response.cpp 
lib/sqlite3.c 
)

target_link_libraries(main PUBLIC)  

add_custom_target(run 
    COMMAND make
    COMMAND main
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Compile and run server"
)