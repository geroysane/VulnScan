#ifndef INTEGRATIONTESTSVULN_H
#define INTEGRATIONTESTSVULN_H

#include <QObject>
#include <QtTest>

/**
 * @class IntegrationTestsVuln
 * @brief Integration tests for vulnerability assessment workflow
 *
 * Tests the complete vulnerability detection and reporting workflow:
 * - CVE database seeding and querying
 * - Vulnerability matching with scan results
 * - SSL/TLS checking integration
 * - Service-specific vulnerability checks
 * - Report generation from vulnerability data
 *
 * These tests verify that all components work together correctly
 * in realistic scenarios.
 */
class IntegrationTestsVuln : public QObject
{
    Q_OBJECT

private slots:
    // End-to-end workflow tests
    void testCompleteVulnerabilityWorkflow();
    void testCveDatabaseAndMatcher();
    void testVulnerabilityScannerWithDatabase();
    void testServiceCheckersIntegration();
    void testReportGenerationWithVulnerabilities();
    void testSslTlsCheckerIntegration();

    // Performance tests
    void testLargeCveDatabasePerformance();
    void testParallelVulnerabilityScanning();
};

#endif // INTEGRATIONTESTSVULN_H
