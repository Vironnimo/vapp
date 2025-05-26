cmake_minimum_required(VERSION 3.20)

find_package(imgui CONFIG REQUIRED)

project(app)

add_executable(app
    app.cpp
    example_class.cpp
    example_fragment.cpp
)

target_link_libraries(app PRIVATE vapp imgui::imgui)