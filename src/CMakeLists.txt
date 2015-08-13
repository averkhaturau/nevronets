cmake_minimum_required(VERSION 3.1)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(nevronets)

add_subdirectory(ga)
add_subdirectory(pneuro)
add_subdirectory(tests)

find_package(MPI)
if(MPI_FOUND)
	include_directories(${MPI_INCLUDE_PATH})
endif(MPI_FOUND)

add_executable(${PROJECT_NAME} ga/main.cpp)
target_link_libraries(${PROJECT_NAME} ga pneuro ${MPI_LIBRARIES})
