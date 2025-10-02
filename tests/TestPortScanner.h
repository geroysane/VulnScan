#ifndef TESTPORTSCANNER_H
#define TESTPORTSCANNER_H

#include <QObject>
#include <QtTest>

/**
 * @class TestPortScanner
 * @brief Unit tests for PortScanner class
 *
 * Tests port scanner functionality including:
 * - Single port scanning
 * - Port range scanning
 * - Parallel scanning
 * - Timeout handling
 * - Cancellation
 * - Progress tracking
 * - Error handling
 */
class TestPortScanner : public QObject {
    Q_OBJECT

private slots:
    // Setup/teardown
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    // Configuration tests
    void testSetTarget();
    void testSetPortRange();
    void testSetPorts();
    void testSetTimeout();
    void testSetThreadCount();

    // Validation tests
    void testValidationEmptyTarget();
    void testValidationNoPorts();
    void testValidationInvalidTimeout();

    // Scanning tests
    void testScanSinglePortOpen();
    void testScanSinglePortClosed();
    void testScanPortRange();
    void testScanCustomPorts();

    // Progress and signals
    void testProgressSignals();
    void testPortFoundSignal();
    void testCompletedSignal();

    // Cancellation
    void testCancellation();
    void testCancelBeforeStart();

    // State management
    void testIsScanning();
    void testCannotStartWhileScanning();
    void testGetResults();
    void testGetOpenPorts();

    // Thread safety
    void testParallelScanning();
    void testMultipleScans();
};

#endif // TESTPORTSCANNER_H
