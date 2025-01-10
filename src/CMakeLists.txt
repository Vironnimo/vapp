find_package(spdlog CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_mixer CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)

# load vconfig directly from git
include(FetchContent)
FetchContent_Declare(
    vconfig
    GIT_REPOSITORY https://github.com/ViroJB/vconfig.git
    GIT_TAG        main
)
FetchContent_MakeAvailable(vconfig)

set(SOURCES
    vapp/vapp.cpp
    vapp/core/entry_point.cpp
    vapp/core/app_params.hpp
    vapp/core/actions.cpp
    vapp/core/event_system.cpp
    vapp/core/resource_manager.cpp
    vapp/core/timer.cpp
    vapp/gui/gui.cpp
    vapp/gui/theme.cpp
    vapp/gui/i_fragment.hpp
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
    SDL2::SDL2
    SDL2_mixer::SDL2_mixer
    SDL2_image::SDL2_image
)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    file(COPY "${CMAKE_SOURCE_DIR}/resources" DESTINATION "${CMAKE_BINARY_DIR}/src/Release")
else(CMAKE_BUILD_TYPE STREQUAL "Debug")
    file(COPY "${CMAKE_SOURCE_DIR}/resources" DESTINATION "${CMAKE_BINARY_DIR}/src/Debug")
endif()