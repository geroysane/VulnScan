#ifndef TESTSCANREPOSITORY_H
#define TESTSCANREPOSITORY_H

#include <QObject>
#include <QtTest>
#include "core/database/ScanRepository.h"
#include "core/database/DatabaseManager.h"

/**
 * @class TestScanRepository
 * @brief Unit tests for ScanRepository class
 */
class TestScanRepository : public QObject {
    Q_OBJECT

private slots:
    // Setup and cleanup
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    // Constructor tests
    void testConstructor();

    // CRUD tests
    void testSave();
    void testFindById();
    void testFindById_NotFound();
    void testUpdate();
    void testRemove();
    void testRemove_NotFound();

    // Query tests
    void testFindAll();
    void testFindByTarget();
    void testFindByStatus();
    void testFindRecent();
    void testFindByDateRange();

    // Count tests
    void testCount();
    void testCount_Empty();

    // Cleanup tests
    void testDeleteOlderThan();

    // Port persistence tests
    void testSaveWithPorts();
    void testLoadPorts();

private:
    ScanRepository *m_repository;
    QString m_testDbPath;

    /**
     * @brief Create a test scan result
     * @param target Target host
     * @param status Scan status
     * @return Test scan result
     */
    ScanResult createTestScan(const QString &target, const QString &status);
};

#endif // TESTSCANREPOSITORY_H
