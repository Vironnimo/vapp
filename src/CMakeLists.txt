find_package(spdlog CONFIG REQUIRED)
find_package(SQLiteCpp CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(CURL REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_mixer CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(portaudio CONFIG REQUIRED)

# vosk
set(VOSK_ROOT "${CMAKE_SOURCE_DIR}/vendor/vosk")
include_directories("${VOSK_ROOT}")

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
    vapp/core/app_settings.hpp
    vapp/core/database.cpp
    vapp/core/actions.cpp
    vapp/core/event_system.cpp
    vapp/core/resource_manager.cpp
    vapp/core/timer.cpp
    vapp/core/logger.cpp
    vapp/core/network.cpp
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
    SQLiteCpp
    imgui::imgui
    CURL::libcurl
    nlohmann_json::nlohmann_json
    glfw
    OpenGL::GL
    portaudio
    vconfig
    SDL2::SDL2
    SDL2_mixer::SDL2_mixer
    SDL2_image::SDL2_image
    "${VOSK_ROOT}/libvosk.lib"
)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    file(COPY "${CMAKE_SOURCE_DIR}/resources" DESTINATION "${CMAKE_BINARY_DIR}/bin/Release")
    # Copy Vosk DLLs
    file(COPY "${VOSK_ROOT}/" DESTINATION "${CMAKE_BINARY_DIR}/bin/Release"
         FILES_MATCHING PATTERN "*.dll")
else(CMAKE_BUILD_TYPE STREQUAL "Debug")
    file(COPY "${CMAKE_SOURCE_DIR}/resources" DESTINATION "${CMAKE_BINARY_DIR}/bin/Debug")
    # Copy Vosk DLLs
    file(COPY "${VOSK_ROOT}/" DESTINATION "${CMAKE_BINARY_DIR}/bin/Debug" 
         FILES_MATCHING PATTERN "*.dll")
endif()