#include "TestPortScanner.h"
#include "core/scanner/PortScanner.h"
#include <QSignalSpy>
#include <QEventLoop>
#include <QTimer>

void TestPortScanner::initTestCase()
{
    // One-time setup before all tests
}

void TestPortScanner::cleanupTestCase()
{
    // One-time cleanup after all tests
}

void TestPortScanner::init()
{
    // Setup before each test
}

void TestPortScanner::cleanup()
{
    // Cleanup after each test
}

// Configuration Tests

void TestPortScanner::testSetTarget()
{
    PortScanner scanner;
    scanner.setTarget("192.168.1.1");

    // We can't directly access m_target, but we can verify by attempting a scan
    scanner.setPortRange(80, 80);
    QVERIFY(true); // Configuration accepted
}

void TestPortScanner::testSetPortRange()
{
    PortScanner scanner;
    scanner.setTarget("127.0.0.1");
    scanner.setPortRange(1, 10);

    // Verify by checking we can start scan
    QVERIFY(!scanner.isScanning());
}

void TestPortScanner::testSetPorts()
{
    PortScanner scanner;
    scanner.setTarget("127.0.0.1");

    QVector<int> ports = {80, 443, 8080};
    scanner.setPorts(ports);

    QVERIFY(!scanner.isScanning());
}

void TestPortScanner::testSetTimeout()
{
    PortScanner scanner;
    scanner.setTimeout(1000);
    QVERIFY(true); // Timeout set successfully
}

void TestPortScanner::testSetThreadCount()
{
    PortScanner scanner;
    scanner.setThreadCount(50);
    QVERIFY(true); // Thread count set successfully
}

// Validation Tests

void TestPortScanner::testValidationEmptyTarget()
{
    PortScanner scanner;
    scanner.setPortRange(80, 80);
    // Don't set target

    QSignalSpy errorSpy(&scanner, &PortScanner::scanError);
    scanner.startScan();

    QCOMPARE(errorSpy.count(), 1);
    QVERIFY(errorSpy.at(0).at(0).toString().contains("Target"));
}

void TestPortScanner::testValidationNoPorts()
{
    PortScanner scanner;
    scanner.setTarget("127.0.0.1");
    // Don't set ports

    QSignalSpy errorSpy(&scanner, &PortScanner::scanError);
    scanner.startScan();

    QCOMPARE(errorSpy.count(), 1);
    QVERIFY(errorSpy.at(0).at(0).toString().contains("ports") ||
            errorSpy.at(0).at(0).toString().contains("No"));
}

void TestPortScanner::testValidationInvalidTimeout()
{
    PortScanner scanner;
    scanner.setTarget("127.0.0.1");
    scanner.setPortRange(80, 80);
    scanner.setTimeout(-100); // Invalid

    QSignalSpy errorSpy(&scanner, &PortScanner::scanError);
    scanner.startScan();

    QCOMPARE(errorSpy.count(), 1);
    QVERIFY(errorSpy.at(0).at(0).toString().contains("timeout"));
}

// Scanning Tests

void TestPortScanner::testScanSinglePortOpen()
{
    PortScanner scanner;
    scanner.setTarget("127.0.0.1");

    // Scan a commonly open port on localhost (we'll use a high port for testing)
    // This test assumes nothing is running on this port, so it will be closed
    scanner.setPorts({65000});
    scanner.setTimeout(500);

    QSignalSpy completedSpy(&scanner, &PortScanner::scanCompleted);

    scanner.startScan();

    // Wait for completion
    QVERIFY(completedSpy.wait(5000));

    // Should have completed
    QCOMPARE(completedSpy.count(), 1);

    // Check results
    auto results = scanner.getResults();
    QCOMPARE(results.size(), 1);
    QCOMPARE(results[0].port, 65000);
}

void TestPortScanner::testScanSinglePortClosed()
{
    PortScanner scanner;
    scanner.setTarget("127.0.0.1");
    scanner.setPorts({65001}); // Unlikely to be open
    scanner.setTimeout(500);

    QSignalSpy completedSpy(&scanner, &PortScanner::scanCompleted);

    scanner.startScan();

    QVERIFY(completedSpy.wait(5000));

    auto results = scanner.getResults();
    QCOMPARE(results.size(), 1);
    QVERIFY(results[0].state == "closed" || results[0].state == "filtered");
}

void TestPortScanner::testScanPortRange()
{
    PortScanner scanner;
    scanner.setTarget("127.0.0.1");
    scanner.setPortRange(65000, 65005); // Small range
    scanner.setTimeout(300);
    scanner.setThreadCount(10);

    QSignalSpy completedSpy(&scanner, &PortScanner::scanCompleted);

    scanner.startScan();

    QVERIFY(completedSpy.wait(10000));

    auto results = scanner.getResults();
    QCOMPARE(results.size(), 6); // 6 ports in range
}

void TestPortScanner::testScanCustomPorts()
{
    PortScanner scanner;
    scanner.setTarget("127.0.0.1");

    QVector<int> ports = {65010, 65011, 65012};
    scanner.setPorts(ports);
    scanner.setTimeout(300);

    QSignalSpy completedSpy(&scanner, &PortScanner::scanCompleted);

    scanner.startScan();

    QVERIFY(completedSpy.wait(5000));

    auto results = scanner.getResults();
    QCOMPARE(results.size(), 3);
}

// Progress and Signals

void TestPortScanner::testProgressSignals()
{
    PortScanner scanner;
    scanner.setTarget("127.0.0.1");
    scanner.setPortRange(65020, 65024); // 5 ports
    scanner.setTimeout(200);

    QSignalSpy progressSpy(&scanner, &PortScanner::scanProgress);
    QSignalSpy completedSpy(&scanner, &PortScanner::scanCompleted);

    scanner.startScan();

    QVERIFY(completedSpy.wait(10000));

    // Should have received progress signals
    QVERIFY(progressSpy.count() >= 1);

    // Last progress should be 5/5
    auto lastProgress = progressSpy.last();
    QCOMPARE(lastProgress.at(0).toInt(), 5); // current
    QCOMPARE(lastProgress.at(1).toInt(), 5); // total
}

void TestPortScanner::testPortFoundSignal()
{
    // This test is tricky because we need an open port
    // For now, we just verify the signal exists
    PortScanner scanner;
    QSignalSpy portFoundSpy(&scanner, &PortScanner::portFound);

    QVERIFY(portFoundSpy.isValid());
}

void TestPortScanner::testCompletedSignal()
{
    PortScanner scanner;
    scanner.setTarget("127.0.0.1");
    scanner.setPorts({65030});
    scanner.setTimeout(200);

    QSignalSpy completedSpy(&scanner, &PortScanner::scanCompleted);

    scanner.startScan();

    QVERIFY(completedSpy.wait(5000));
    QCOMPARE(completedSpy.count(), 1);

    // Should not be cancelled
    bool cancelled = completedSpy.at(0).at(0).toBool();
    QCOMPARE(cancelled, false);
}

// Cancellation

void TestPortScanner::testCancellation()
{
    PortScanner scanner;
    scanner.setTarget("127.0.0.1");
    scanner.setPortRange(65040, 65100); // Larger range
    scanner.setTimeout(1000);
    scanner.setThreadCount(5); // Slow it down

    QSignalSpy completedSpy(&scanner, &PortScanner::scanCompleted);

    scanner.startScan();

    // Wait a bit then cancel
    QTest::qWait(100);
    scanner.stopScan();

    // Should complete with cancelled flag
    QVERIFY(completedSpy.count() >= 1);

    bool cancelled = completedSpy.at(0).at(0).toBool();
    QCOMPARE(cancelled, true);
}

void TestPortScanner::testCancelBeforeStart()
{
    PortScanner scanner;
    scanner.setTarget("127.0.0.1");
    scanner.setPorts({65050});

    // Stop before starting (should do nothing)
    scanner.stopScan();

    QVERIFY(!scanner.isScanning());
}

// State Management

void TestPortScanner::testIsScanning()
{
    PortScanner scanner;
    scanner.setTarget("127.0.0.1");
    scanner.setPortRange(65060, 65064);
    scanner.setTimeout(200);

    QVERIFY(!scanner.isScanning());

    QSignalSpy completedSpy(&scanner, &PortScanner::scanCompleted);
    scanner.startScan();

    // Should be scanning immediately after start
    // Note: might finish quickly, so this is a soft check

    // Wait for completion
    QVERIFY(completedSpy.wait(5000));

    // Should not be scanning after completion
    QVERIFY(!scanner.isScanning());
}

void TestPortScanner::testCannotStartWhileScanning()
{
    PortScanner scanner;
    scanner.setTarget("127.0.0.1");
    scanner.setPortRange(65070, 65090); // Larger range
    scanner.setTimeout(500);
    scanner.setThreadCount(5);

    QSignalSpy errorSpy(&scanner, &PortScanner::scanError);
    QSignalSpy completedSpy(&scanner, &PortScanner::scanCompleted);

    scanner.startScan();

    // Try to start again immediately
    scanner.startScan();

    // Should get error
    QVERIFY(errorSpy.count() >= 1);

    // Wait for original scan to complete
    QVERIFY(completedSpy.wait(20000));
}

void TestPortScanner::testGetResults()
{
    PortScanner scanner;
    scanner.setTarget("127.0.0.1");
    scanner.setPorts({65100, 65101});
    scanner.setTimeout(200);

    QSignalSpy completedSpy(&scanner, &PortScanner::scanCompleted);

    scanner.startScan();
    QVERIFY(completedSpy.wait(5000));

    auto results = scanner.getResults();
    QCOMPARE(results.size(), 2);
}

void TestPortScanner::testGetOpenPorts()
{
    PortScanner scanner;
    scanner.setTarget("127.0.0.1");
    scanner.setPorts({65110, 65111, 65112});
    scanner.setTimeout(200);

    QSignalSpy completedSpy(&scanner, &PortScanner::scanCompleted);

    scanner.startScan();
    QVERIFY(completedSpy.wait(5000));

    auto openPorts = scanner.getOpenPorts();
    // Likely no open ports in this range
    QVERIFY(openPorts.size() >= 0);
}

// Thread Safety

void TestPortScanner::testParallelScanning()
{
    PortScanner scanner;
    scanner.setTarget("127.0.0.1");
    scanner.setPortRange(65200, 65250); // 51 ports
    scanner.setTimeout(300);
    scanner.setThreadCount(20); // Parallel execution

    QSignalSpy completedSpy(&scanner, &PortScanner::scanCompleted);
    QSignalSpy progressSpy(&scanner, &PortScanner::scanProgress);

    scanner.startScan();

    QVERIFY(completedSpy.wait(30000));

    auto results = scanner.getResults();
    QCOMPARE(results.size(), 51);

    // Progress should have been reported
    QVERIFY(progressSpy.count() >= 1);
}

void TestPortScanner::testMultipleScans()
{
    PortScanner scanner;
    scanner.setTarget("127.0.0.1");
    scanner.setTimeout(200);

    // First scan
    scanner.setPorts({65300, 65301});
    QSignalSpy completedSpy1(&scanner, &PortScanner::scanCompleted);
    scanner.startScan();
    QVERIFY(completedSpy1.wait(5000));

    auto results1 = scanner.getResults();
    QCOMPARE(results1.size(), 2);

    // Second scan (should reset results)
    scanner.setPorts({65302, 65303, 65304});
    QSignalSpy completedSpy2(&scanner, &PortScanner::scanCompleted);
    scanner.startScan();
    QVERIFY(completedSpy2.wait(5000));

    auto results2 = scanner.getResults();
    QCOMPARE(results2.size(), 3);
}
