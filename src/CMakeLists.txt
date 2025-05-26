find_package(spdlog CONFIG REQUIRED)
find_package(SQLiteCpp CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(CURL REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)

# vosk
set(VOSK_ROOT "${CMAKE_SOURCE_DIR}/vendor/vosk")
include_directories("${VOSK_ROOT}")

set(SOURCES
    vapp/vapp.cpp
    vapp/core/app_settings.hpp
    vapp/core/database.cpp
    vapp/core/actions.cpp
    vapp/core/event_system.cpp
    vapp/core/resource_manager.cpp
    vapp/core/timer.cpp
    vapp/core/network.cpp
    vapp/core/audio_recorder.cpp
    vapp/gui/gui.cpp
    vapp/gui/theme.cpp
    vapp/gui/i_fragment.hpp
    vapp/core/speech_to_text.cpp
    vapp/core/ai_request.cpp
)

add_library(${PROJECT_NAME} STATIC
    ${SOURCES}
)

target_link_libraries(${PROJECT_NAME} PUBLIC 
    spdlog::spdlog
    SQLiteCpp
    imgui::imgui
    CURL::libcurl
    nlohmann_json::nlohmann_json
    glfw
    OpenGL::GL
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