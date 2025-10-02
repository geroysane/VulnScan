#ifndef TESTSERVICEVULNCHECKER_H
#define TESTSERVICEVULNCHECKER_H

#include <QObject>
#include <QtTest>
#include "core/vulnerability/ServiceVulnChecker.h"
#include "core/vulnerability/HttpVulnChecker.h"
#include "core/vulnerability/SshVulnChecker.h"
#include "core/vulnerability/FtpVulnChecker.h"

/**
 * @class TestServiceVulnChecker
 * @brief Unit tests for service-specific vulnerability checkers
 *
 * Tests the plugin-based vulnerability detection system including:
 * - ServiceVulnChecker base class functionality
 * - HttpVulnChecker for web vulnerabilities
 * - SshVulnChecker for SSH vulnerabilities
 * - FtpVulnChecker for FTP vulnerabilities
 */
class TestServiceVulnChecker : public QObject
{
    Q_OBJECT

private slots:
    // Base class tests
    void testServiceVulnCheckerTimeout();
    void testServiceVulnCheckerCreateVulnerability();

    // HttpVulnChecker tests
    void testHttpCheckerCanHandle();
    void testHttpCheckerName();
    void testHttpCheckerDescription();
    void testHttpCheckerSecurityHeaders();
    void testHttpCheckerDangerousMethods();
    void testHttpCheckerInformationDisclosure();

    // SshVulnChecker tests
    void testSshCheckerCanHandle();
    void testSshCheckerName();
    void testSshCheckerDescription();
    void testSshCheckerVersionExtraction();
    void testSshCheckerVersionComparison();
    void testSshCheckerProtocolV1Detection();
    void testSshCheckerOutdatedVersion();

    // FtpVulnChecker tests
    void testFtpCheckerCanHandle();
    void testFtpCheckerName();
    void testFtpCheckerDescription();
    void testFtpCheckerVersionExtraction();
    void testFtpCheckerPlainTextAuth();
    void testFtpCheckerBannerDisclosure();

    // Integration tests (optional - require network access)
    void testHttpCheckerRealScan_data();
    void testHttpCheckerRealScan();
    void testSshCheckerRealScan_data();
    void testSshCheckerRealScan();
};

#endif // TESTSERVICEVULNCHECKER_H
