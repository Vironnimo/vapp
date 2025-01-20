cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(vapp)

include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/vendor)

# enables multi threading
if(MSVC)
    add_compile_options(/MP)
endif()

add_subdirectory(src)

enable_testing()
add_subdirectory(tests)

# needed for static
# if(CMAKE_BUILD_TYPE STREQUAL "Debug")
#     target_compile_options(todo_app PRIVATE /MTd)  # Static runtime for Debug
# elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
#     target_compile_options(todo_app PRIVATE /MT)   # Static runtime for Release
# endif()
