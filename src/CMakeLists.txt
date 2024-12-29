find_package(spdlog CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)

# [cmake]     find_package(spdlog CONFIG REQUIRED)
# [cmake]     target_link_libraries(main PRIVATE spdlog::spdlog)

# FetchContent setup
include(FetchContent)
FetchContent_Declare(
    vconfig
    GIT_REPOSITORY https://github.com/ViroJB/vconfig.git
    GIT_TAG        main
)
FetchContent_MakeAvailable(vconfig)

set(SOURCES
    vapp/core/entry_point.cpp
    vapp/core/app_params.hpp
    vapp/core/actions.cpp
    vapp/core/event_system.cpp
    vapp/vapp.cpp
    vapp/gui/gui.cpp
    vapp/gui/gui_style.cpp
    vapp/gui/fragment/i_base_fragment.hpp
    app/app.cpp
    app/example_class.cpp
    app/example_fragment.cpp
)

add_executable(${PROJECT_NAME} 
    ${SOURCES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    spdlog::spdlog
    imgui::imgui
    glfw
    OpenGL::GL
    vconfig
)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    file(COPY "${CMAKE_SOURCE_DIR}/fonts" DESTINATION "${CMAKE_BINARY_DIR}/src/Release")
else(CMAKE_BUILD_TYPE STREQUAL "Debug")
    file(COPY "${CMAKE_SOURCE_DIR}/fonts" DESTINATION "${CMAKE_BINARY_DIR}/src/Debug")
endif()

# file(COPY "${CMAKE_SOURCE_DIR}/fonts" DESTINATION "${CMAKE_BINARY_DIR}/src/Debug")
# file(COPY "${CMAKE_SOURCE_DIR}/fonts" DESTINATION "${CMAKE_BINARY_DIR}/src/Release")