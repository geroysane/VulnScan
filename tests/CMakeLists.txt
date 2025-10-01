add_executable(vulnscan_tests
    main.cpp
    TestDatabaseManager.cpp
    TestDatabaseManager.h
    TestConfiguration.cpp
    TestConfiguration.h
)

target_link_libraries(vulnscan_tests
    PRIVATE
        vulnscan_core
        Qt6::Test
)

# Add test to CTest
add_test(NAME VulnScanTests COMMAND vulnscan_tests)
