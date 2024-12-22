find_package(GTest CONFIG REQUIRED)

set(TEST_SOURCES
    main.cpp
    example_test.cpp
)

add_executable(${PROJECT_NAME}_tests
    ${TEST_SOURCES}
)

target_link_libraries(${PROJECT_NAME}_tests PRIVATE 
    GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main
)

add_test(NAME AllTests COMMAND ${PROJECT_NAME}_tests)