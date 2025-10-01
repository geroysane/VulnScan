TEMPLATE = lib
CONFIG += staticlib
TARGET = vulnscan_core

include(../../common.pri)

QT -= gui widgets

HEADERS += \
    scanner/IScanner.h \
    scanner/ScanResult.h \
    scanner/PortResult.h \
    scanner/ServiceInfo.h \
    scanner/Vulnerability.h \
    ai/IAIProvider.h \
    ai/AIAnalysis.h \
    database/IRepository.h \
    database/DatabaseManager.h \
    scanner/IVulnerabilityChecker.h \
    report/IReportGenerator.h \
    report/ReportData.h \
    config/Configuration.h \
    common/Logger.h

SOURCES += \
    database/DatabaseManager.cpp \
    config/Configuration.cpp \
    common/Logger.cpp
