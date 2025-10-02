add_library(vulnscan_core STATIC
    # Database
    database/DatabaseManager.cpp
    database/DatabaseManager.h
    database/IRepository.h
    database/ScanRepository.cpp
    database/ScanRepository.h

    # Config
    config/Configuration.cpp
    config/Configuration.h

    # Common
    common/Logger.cpp
    common/Logger.h

    # Interfaces
    scanner/IScanner.h
    scanner/IVulnerabilityChecker.h
    ai/IAIProvider.h
    report/IReportGenerator.h

    # Data Models
    scanner/ScanResult.h
    scanner/PortResult.h
    scanner/ServiceInfo.h
    scanner/Vulnerability.h
    ai/AIAnalysis.h
    report/ReportData.h

    # Scanner Implementation
    scanner/PortScanner.cpp
    scanner/PortScanner.h
    scanner/ScanWorker.cpp
    scanner/ScanWorker.h
    scanner/ServiceDetector.cpp
    scanner/ServiceDetector.h
    scanner/DnsResolver.cpp
    scanner/DnsResolver.h
    scanner/NetworkInterface.cpp
    scanner/NetworkInterface.h
    scanner/ScanConfiguration.cpp
    scanner/ScanConfiguration.h
    scanner/ScanEngine.cpp
    scanner/ScanEngine.h
)

target_link_libraries(vulnscan_core
    PUBLIC
        Qt6::Core
        Qt6::Network
        Qt6::Sql
)

target_include_directories(vulnscan_core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/..
)
