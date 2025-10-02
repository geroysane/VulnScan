add_executable(vulnscan_tests
    main.cpp
    TestDatabaseManager.cpp
    TestDatabaseManager.h
    TestConfiguration.cpp
    TestConfiguration.h
    TestPortScanner.cpp
    TestPortScanner.h
    TestServiceDetector.cpp
    TestServiceDetector.h
    TestDnsResolver.cpp
    TestDnsResolver.h
    TestNetworkInterface.cpp
    TestNetworkInterface.h
    TestScanEngine.cpp
    TestScanEngine.h
    TestScanRepository.cpp
    TestScanRepository.h
)

target_link_libraries(vulnscan_tests
    PRIVATE
        vulnscan_core
        Qt6::Test
)

# Add test to CTest
add_test(NAME VulnScanTests COMMAND vulnscan_tests)
