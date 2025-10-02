#include "TestScanRepository.h"
#include <QDir>
#include <QFile>

void TestScanRepository::initTestCase()
{
    // Create temporary database for testing
    m_testDbPath = QDir::tempPath() + "/test_vulnscan.db";

    // Remove old test database if exists
    QFile::remove(m_testDbPath);

    // Initialize database
    QVERIFY(DatabaseManager::instance().initialize(m_testDbPath));
}

void TestScanRepository::cleanupTestCase()
{
    DatabaseManager::instance().close();
    QFile::remove(m_testDbPath);
}

void TestScanRepository::init()
{
    m_repository = new ScanRepository();
}

void TestScanRepository::cleanup()
{
    delete m_repository;
    m_repository = nullptr;
}

void TestScanRepository::testConstructor()
{
    QVERIFY(m_repository != nullptr);
}

ScanResult TestScanRepository::createTestScan(const QString &target, const QString &status)
{
    ScanResult scan;
    scan.targetHost = target;
    scan.scanStartTime = QDateTime::currentDateTime();
    scan.scanEndTime = QDateTime::currentDateTime().addSecs(60);
    scan.status = status;

    // Add some test ports
    PortResult port1;
    port1.port = 80;
    port1.protocol = "TCP";
    port1.state = "open";
    port1.service = "http";
    port1.version = "Apache 2.4";

    PortResult port2;
    port2.port = 443;
    port2.protocol = "TCP";
    port2.state = "open";
    port2.service = "https";
    port2.version = "nginx 1.18";

    scan.ports.append(port1);
    scan.ports.append(port2);

    return scan;
}

void TestScanRepository::testSave()
{
    ScanResult scan = createTestScan("192.168.1.1", "completed");

    m_repository->save(scan);

    // Verify it was saved by counting
    int count = m_repository->count();
    QVERIFY(count > 0);
}

void TestScanRepository::testFindById()
{
    // Save a scan first
    ScanResult scan = createTestScan("10.0.0.1", "completed");
    m_repository->save(scan);

    // Get all scans to find the ID
    QList<ScanResult> all = m_repository->findAll();
    QVERIFY(!all.isEmpty());

    int scanId = all.first().id;

    // Find by ID
    std::optional<ScanResult> foundOpt = m_repository->findById(scanId);

    QVERIFY(foundOpt.has_value());
    ScanResult found = foundOpt.value();
    QCOMPARE(found.id, scanId);
    QCOMPARE(found.targetHost, QString("10.0.0.1"));
    QCOMPARE(found.status, QString("completed"));
    QVERIFY(!found.ports.isEmpty());
}

void TestScanRepository::testFindById_NotFound()
{
    std::optional<ScanResult> notFound = m_repository->findById(99999);
    QVERIFY(!notFound.has_value());
}

void TestScanRepository::testUpdate()
{
    // Save a scan
    ScanResult scan = createTestScan("172.16.0.1", "running");
    m_repository->save(scan);

    // Get the saved scan
    QList<ScanResult> all = m_repository->findAll();
    QVERIFY(!all.isEmpty());

    ScanResult saved = all.first();
    int scanId = saved.id;

    // Update it
    saved.status = "completed";
    saved.scanEndTime = QDateTime::currentDateTime();

    bool updated = m_repository->update(saved);
    QVERIFY(updated);

    // Verify update
    std::optional<ScanResult> foundOpt = m_repository->findById(scanId);
    QVERIFY(foundOpt.has_value());
    QCOMPARE(foundOpt.value().status, QString("completed"));
}

void TestScanRepository::testRemove()
{
    // Save a scan
    ScanResult scan = createTestScan("192.168.2.1", "completed");
    m_repository->save(scan);

    int countBefore = m_repository->count();

    // Get ID
    QList<ScanResult> all = m_repository->findAll();
    int scanId = all.last().id;

    // Remove it
    m_repository->remove(scanId);

    int countAfter = m_repository->count();
    QCOMPARE(countAfter, countBefore - 1);

    // Verify it's gone
    std::optional<ScanResult> notFound = m_repository->findById(scanId);
    QVERIFY(!notFound.has_value());
}

void TestScanRepository::testRemove_NotFound()
{
    m_repository->remove(99999);
    QVERIFY(true); // Just verify it doesn't crash
}

void TestScanRepository::testFindAll()
{
    // Save multiple scans
    m_repository->save(createTestScan("192.168.1.1", "completed"));
    m_repository->save(createTestScan("192.168.1.2", "completed"));
    m_repository->save(createTestScan("192.168.1.3", "failed"));

    QList<ScanResult> all = m_repository->findAll();

    QVERIFY(all.size() >= 3);
}

void TestScanRepository::testFindByTarget()
{
    QString target = "192.168.100.50";
    m_repository->save(createTestScan(target, "completed"));
    m_repository->save(createTestScan(target, "completed"));
    m_repository->save(createTestScan("192.168.100.51", "completed"));

    QList<ScanResult> results = m_repository->findByTarget(target);

    QVERIFY(results.size() >= 2);
    for (const ScanResult &scan : results) {
        QCOMPARE(scan.targetHost, target);
    }
}

void TestScanRepository::testFindByStatus()
{
    m_repository->save(createTestScan("10.0.0.1", "completed"));
    m_repository->save(createTestScan("10.0.0.2", "failed"));
    m_repository->save(createTestScan("10.0.0.3", "completed"));

    QList<ScanResult> completed = m_repository->findByStatus("completed");

    QVERIFY(completed.size() >= 2);
    for (const ScanResult &scan : completed) {
        QCOMPARE(scan.status, QString("completed"));
    }
}

void TestScanRepository::testFindRecent()
{
    m_repository->save(createTestScan("10.1.1.1", "completed"));
    m_repository->save(createTestScan("10.1.1.2", "completed"));
    m_repository->save(createTestScan("10.1.1.3", "completed"));

    QList<ScanResult> recent = m_repository->findRecent(2);

    QVERIFY(recent.size() <= 2);
}

void TestScanRepository::testFindByDateRange()
{
    QDateTime now = QDateTime::currentDateTime();
    QDateTime start = now.addDays(-1);
    QDateTime end = now.addDays(1);

    m_repository->save(createTestScan("10.2.2.1", "completed"));

    QList<ScanResult> results = m_repository->findByDateRange(start, end);

    QVERIFY(!results.isEmpty());
}

void TestScanRepository::testCount()
{
    int initialCount = m_repository->count();

    m_repository->save(createTestScan("10.3.3.1", "completed"));
    m_repository->save(createTestScan("10.3.3.2", "completed"));

    int newCount = m_repository->count();

    QCOMPARE(newCount, initialCount + 2);
}

void TestScanRepository::testCount_Empty()
{
    // Count should work even with no scans (just returns current count)
    int count = m_repository->count();
    QVERIFY(count >= 0);
}

void TestScanRepository::testDeleteOlderThan()
{
    // This test just verifies the method doesn't crash
    // In a real scenario, we'd need to manipulate scan dates
    int deleted = m_repository->deleteOlderThan(365);
    QVERIFY(deleted >= 0);
}

void TestScanRepository::testSaveWithPorts()
{
    ScanResult scan = createTestScan("192.168.50.1", "completed");
    QCOMPARE(scan.ports.size(), 2);

    m_repository->save(scan);

    // Retrieve and verify ports were saved
    QList<ScanResult> all = m_repository->findAll();
    bool found = false;
    for (const ScanResult &s : all) {
        if (s.targetHost == "192.168.50.1") {
            QCOMPARE(s.ports.size(), 2);
            QCOMPARE(s.ports[0].port, 80);
            QCOMPARE(s.ports[1].port, 443);
            found = true;
            break;
        }
    }
    QVERIFY(found);
}

void TestScanRepository::testLoadPorts()
{
    ScanResult scan = createTestScan("192.168.60.1", "completed");
    m_repository->save(scan);

    QList<ScanResult> all = m_repository->findAll();
    for (const ScanResult &s : all) {
        if (s.targetHost == "192.168.60.1") {
            QVERIFY(!s.ports.isEmpty());
            QVERIFY(s.ports.size() == 2);
            break;
        }
    }
}
