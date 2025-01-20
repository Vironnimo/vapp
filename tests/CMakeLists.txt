find_package(GTest CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(CURL REQUIRED)
find_package(OpenGL REQUIRED)
find_package(SQLiteCpp CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_mixer CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)

if (MSVC)
    add_compile_options(/utf-8)
endif()

set(TEST_SOURCES
    main.cpp
    action_tests.cpp
    ../src/vapp/core/actions.cpp
    database_tests.cpp
    ../src/vapp/core/database.cpp
    event_tests.cpp
    ../src/vapp/core/event_system.cpp
    network_tests.cpp
    ../src/vapp/core/network.cpp
    resource_manager_tests.cpp
    ../src/vapp/core/resource_manager.cpp
)

add_executable(${PROJECT_NAME}_tests
    ${TEST_SOURCES}
)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    file(COPY "${CMAKE_SOURCE_DIR}/tests/resources" DESTINATION "${CMAKE_BINARY_DIR}/tests/Release")
else(CMAKE_BUILD_TYPE STREQUAL "Debug")
    file(COPY "${CMAKE_SOURCE_DIR}/tests/resources" DESTINATION "${CMAKE_BINARY_DIR}/tests/Debug")
endif()

target_link_libraries(${PROJECT_NAME}_tests PRIVATE 
    GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main
    glfw
    OpenGL::GL
    spdlog::spdlog
    CURL::libcurl
    nlohmann_json::nlohmann_json
    SQLiteCpp
    SDL2::SDL2
    SDL2_mixer::SDL2_mixer
    SDL2_image::SDL2_image
)

add_test(NAME AllTests COMMAND ${PROJECT_NAME}_tests)

set_tests_properties(AllTests PROPERTIES WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/tests/Debug)