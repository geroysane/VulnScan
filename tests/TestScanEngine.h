#ifndef TESTSCANENGINE_H
#define TESTSCANENGINE_H

#include <QObject>
#include <QtTest>
#include "core/scanner/ScanEngine.h"
#include "core/scanner/ScanConfiguration.h"

/**
 * @class TestScanEngine
 * @brief Unit tests for ScanEngine class
 */
class TestScanEngine : public QObject {
    Q_OBJECT

private slots:
    // Setup and cleanup
    void init();
    void cleanup();

    // Constructor tests
    void testConstructor();

    // Configuration tests
    void testScanConfiguration_Valid();
    void testScanConfiguration_Invalid();
    void testScanConfiguration_QuickScan();
    void testScanConfiguration_FullScan();
    void testScanConfiguration_CustomScan();

    // Port count tests
    void testGetPortCount_Range();
    void testGetPortCount_SpecificPorts();

    // Scan engine tests
    void testStartScan_ValidConfig();
    void testStartScan_InvalidConfig();
    void testStartScan_AlreadyRunning();
    void testIsScanning();
    void testCancelScan();

    // Signal tests
    void testSignals_ScanStarted();
    void testSignals_ScanProgress();
    void testSignals_ScanCompleted();

    // Workflow tests
    void testScanWorkflow_PortScanOnly();
    void testScanWorkflow_WithServiceDetection();

    // Error handling tests
    void testErrorHandling_InvalidTarget();
    void testErrorHandling_InvalidPortRange();

private:
    ScanEngine *m_engine;
};

#endif // TESTSCANENGINE_H
