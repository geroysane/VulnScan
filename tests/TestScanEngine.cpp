#include "TestScanEngine.h"
#include <QSignalSpy>

void TestScanEngine::init()
{
    m_engine = new ScanEngine();
}

void TestScanEngine::cleanup()
{
    if (m_engine->isScanning()) {
        m_engine->cancelScan();
        QTest::qWait(100); // Wait for cancellation
    }
    delete m_engine;
    m_engine = nullptr;
}

void TestScanEngine::testConstructor()
{
    QVERIFY(m_engine != nullptr);
    QVERIFY(!m_engine->isScanning());
}

void TestScanEngine::testScanConfiguration_Valid()
{
    ScanConfiguration config;
    config.target = "127.0.0.1";
    config.startPort = 80;
    config.endPort = 443;
    config.timeout = 1000;
    config.maxThreads = 50;

    QVERIFY(config.isValid());
}

void TestScanEngine::testScanConfiguration_Invalid()
{
    // Empty target
    ScanConfiguration config1;
    QVERIFY(!config1.isValid());

    // Invalid port range
    ScanConfiguration config2;
    config2.target = "127.0.0.1";
    config2.startPort = 100;
    config2.endPort = 50;
    QVERIFY(!config2.isValid());

    // Invalid timeout
    ScanConfiguration config3;
    config3.target = "127.0.0.1";
    config3.timeout = -1;
    QVERIFY(!config3.isValid());

    // Invalid thread count
    ScanConfiguration config4;
    config4.target = "127.0.0.1";
    config4.maxThreads = 0;
    QVERIFY(!config4.isValid());
}

void TestScanEngine::testScanConfiguration_QuickScan()
{
    ScanConfiguration config = ScanConfiguration::quickScan("192.168.1.1");

    QCOMPARE(config.target, QString("192.168.1.1"));
    QCOMPARE(config.startPort, 1);
    QCOMPARE(config.endPort, 1000);
    QCOMPARE(config.scanType, ScanConfiguration::PortScan);
    QVERIFY(!config.enableServiceDetection);
    QVERIFY(config.isValid());
}

void TestScanEngine::testScanConfiguration_FullScan()
{
    ScanConfiguration config = ScanConfiguration::fullScan("10.0.0.1");

    QCOMPARE(config.target, QString("10.0.0.1"));
    QCOMPARE(config.startPort, 1);
    QCOMPARE(config.endPort, 65535);
    QCOMPARE(config.scanType, ScanConfiguration::FullScan);
    QVERIFY(config.enableServiceDetection);
    QVERIFY(config.isValid());
}

void TestScanEngine::testScanConfiguration_CustomScan()
{
    ScanConfiguration config = ScanConfiguration::customScan("172.16.0.1", 1000, 2000);

    QCOMPARE(config.target, QString("172.16.0.1"));
    QCOMPARE(config.startPort, 1000);
    QCOMPARE(config.endPort, 2000);
    QVERIFY(config.isValid());
}

void TestScanEngine::testGetPortCount_Range()
{
    ScanConfiguration config;
    config.startPort = 1;
    config.endPort = 100;

    QCOMPARE(config.getPortCount(), 100);
}

void TestScanEngine::testGetPortCount_SpecificPorts()
{
    ScanConfiguration config;
    config.specificPorts = {80, 443, 8080, 8443};

    QCOMPARE(config.getPortCount(), 4);
}

void TestScanEngine::testStartScan_ValidConfig()
{
    ScanConfiguration config = ScanConfiguration::quickScan("127.0.0.1");
    config.endPort = 85; // Small range for quick test

    bool started = m_engine->startScan(config);
    QVERIFY(started);
    QVERIFY(m_engine->isScanning());
}

void TestScanEngine::testStartScan_InvalidConfig()
{
    ScanConfiguration config;
    // Empty target - invalid

    bool started = m_engine->startScan(config);
    QVERIFY(!started);
    QVERIFY(!m_engine->isScanning());
}

void TestScanEngine::testStartScan_AlreadyRunning()
{
    ScanConfiguration config = ScanConfiguration::quickScan("127.0.0.1");
    config.endPort = 85;

    // Start first scan
    bool started1 = m_engine->startScan(config);
    QVERIFY(started1);

    // Try to start second scan while first is running
    bool started2 = m_engine->startScan(config);
    QVERIFY(!started2);
}

void TestScanEngine::testIsScanning()
{
    QVERIFY(!m_engine->isScanning());

    ScanConfiguration config = ScanConfiguration::quickScan("127.0.0.1");
    config.endPort = 85;

    m_engine->startScan(config);
    QVERIFY(m_engine->isScanning());

    // Wait for scan to complete or cancel it
    m_engine->cancelScan();
    QTest::qWait(100);
    QVERIFY(!m_engine->isScanning());
}

void TestScanEngine::testCancelScan()
{
    ScanConfiguration config = ScanConfiguration::quickScan("127.0.0.1");
    config.endPort = 100;

    m_engine->startScan(config);
    QVERIFY(m_engine->isScanning());

    m_engine->cancelScan();
    QTest::qWait(100);

    QVERIFY(!m_engine->isScanning());

    ScanResult result = m_engine->getCurrentResult();
    QCOMPARE(result.status, QString("cancelled"));
}

void TestScanEngine::testSignals_ScanStarted()
{
    QSignalSpy spy(m_engine, &ScanEngine::scanStarted);

    ScanConfiguration config = ScanConfiguration::quickScan("127.0.0.1");
    config.endPort = 85;

    m_engine->startScan(config);

    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), QString("127.0.0.1"));
}

void TestScanEngine::testSignals_ScanProgress()
{
    QSignalSpy spy(m_engine, &ScanEngine::scanProgress);

    ScanConfiguration config = ScanConfiguration::quickScan("127.0.0.1");
    config.endPort = 85;

    m_engine->startScan(config);

    // Wait a bit for progress signals
    QTest::qWait(500);

    // Should have received at least one progress signal
    QVERIFY(spy.count() >= 1);
}

void TestScanEngine::testSignals_ScanCompleted()
{
    QSignalSpy completedSpy(m_engine, &ScanEngine::scanCompleted);
    QSignalSpy failedSpy(m_engine, &ScanEngine::scanFailed);

    ScanConfiguration config = ScanConfiguration::quickScan("127.0.0.1");
    config.startPort = 80;
    config.endPort = 85; // Very small range

    m_engine->startScan(config);

    // Wait for scan to complete (should be quick with small range)
    bool finished = completedSpy.wait(10000) || failedSpy.wait(10000);

    QVERIFY(finished);
    // Either completed or failed is acceptable (depends on network state)
    QVERIFY(completedSpy.count() + failedSpy.count() >= 1);
}

void TestScanEngine::testScanWorkflow_PortScanOnly()
{
    QSignalSpy completedSpy(m_engine, &ScanEngine::scanCompleted);

    ScanConfiguration config;
    config.target = "127.0.0.1";
    config.startPort = 80;
    config.endPort = 82;
    config.timeout = 500;
    config.enableServiceDetection = false;

    m_engine->startScan(config);

    // Wait for completion
    completedSpy.wait(10000);

    if (completedSpy.count() > 0) {
        ScanResult result = m_engine->getCurrentResult();
        QCOMPARE(result.status, QString("completed"));
        QCOMPARE(result.targetHost, QString("127.0.0.1"));
    }
}

void TestScanEngine::testScanWorkflow_WithServiceDetection()
{
    QSignalSpy completedSpy(m_engine, &ScanEngine::scanCompleted);

    ScanConfiguration config;
    config.target = "127.0.0.1";
    config.startPort = 80;
    config.endPort = 82;
    config.timeout = 500;
    config.enableServiceDetection = true;

    m_engine->startScan(config);

    // Wait for completion
    completedSpy.wait(10000);

    if (completedSpy.count() > 0) {
        ScanResult result = m_engine->getCurrentResult();
        QCOMPARE(result.status, QString("completed"));
    }
}

void TestScanEngine::testErrorHandling_InvalidTarget()
{
    QSignalSpy failedSpy(m_engine, &ScanEngine::scanFailed);

    ScanConfiguration config;
    config.target = ""; // Invalid
    config.startPort = 80;
    config.endPort = 100;

    bool started = m_engine->startScan(config);

    QVERIFY(!started);
    QCOMPARE(failedSpy.count(), 1);
}

void TestScanEngine::testErrorHandling_InvalidPortRange()
{
    QSignalSpy failedSpy(m_engine, &ScanEngine::scanFailed);

    ScanConfiguration config;
    config.target = "127.0.0.1";
    config.startPort = 100;
    config.endPort = 50; // Invalid: end < start

    bool started = m_engine->startScan(config);

    QVERIFY(!started);
    QCOMPARE(failedSpy.count(), 1);
}
