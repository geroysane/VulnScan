#ifndef INTEGRATIONTESTS_H
#define INTEGRATIONTESTS_H

#include <QObject>
#include <QTest>
#include <QSharedPointer>
#include "../src/core/scanner/ScanEngine.h"
#include "../src/core/database/ScanRepository.h"
#include "../src/core/database/DatabaseManager.h"

/**
 * @brief Integration tests for the complete VulnScan system
 *
 * Tests the full workflow including:
 * - End-to-end scanning
 * - Database persistence
 * - Component integration
 * - Thread safety
 * - Performance
 */
class IntegrationTests : public QObject
{
    Q_OBJECT

private slots:
    // Test lifecycle
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    // End-to-end workflow tests
    void testCompleteQuickScanWorkflow();
    void testCompleteFullScanWorkflow();
    void testCustomScanWorkflow();
    void testScanWithServiceDetection();
    void testScanWithDnsResolution();
    void testMultipleTargetScans();

    // Database integration tests
    void testScanPersistence();
    void testScanHistoryRetrieval();
    void testPortResultsPersistence();
    void testConcurrentDatabaseAccess();

    // Component integration tests
    void testPortScannerIntegration();
    void testServiceDetectorIntegration();
    void testDnsResolverIntegration();
    void testNetworkInterfaceIntegration();

    // Thread safety tests
    void testConcurrentScans();
    void testScanCancellationThreadSafety();
    void testRepositoryThreadSafety();

    // Performance tests
    void testLargePortRangePerformance();
    void testMultipleHostsPerformance();
    void testDatabaseQueryPerformance();

    // Error handling tests
    void testInvalidTargetHandling();
    void testNetworkFailureHandling();
    void testDatabaseErrorHandling();

private:
    QString m_testDbPath;
    QSharedPointer<ScanRepository> m_repository;
    QSharedPointer<ScanEngine> m_scanEngine;

    // Helper methods
    ScanResult executeBlockingScan(const ScanConfiguration &config, int timeoutMs = 30000);
    bool waitForScanCompletion(int timeoutMs = 30000);
    void cleanupDatabase();
};

#endif // INTEGRATIONTESTS_H
