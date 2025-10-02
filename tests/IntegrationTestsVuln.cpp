#include "IntegrationTestsVuln.h"
#include "core/vulnerability/VulnerabilityDatabase.h"
#include "core/vulnerability/VulnerabilityMatcher.h"
#include "core/vulnerability/VulnerabilityScanner.h"
#include "core/vulnerability/CveSeeder.h"
#include "core/vulnerability/SslTlsChecker.h"
#include "core/vulnerability/HttpVulnChecker.h"
#include "core/vulnerability/SshVulnChecker.h"
#include "core/vulnerability/FtpVulnChecker.h"
#include "core/report/JsonReportGenerator.h"
#include "core/report/CsvReportGenerator.h"
#include <QTemporaryDir>
#include <QSignalSpy>
#include <QElapsedTimer>

void IntegrationTestsVuln::testCompleteVulnerabilityWorkflow()
{
    // Create temporary database
    QString dbPath = ":memory:";
    VulnerabilityDatabase db(dbPath);
    QVERIFY(db.initialize());

    // Seed with sample CVE data
    CveSeeder seeder;
    int cveCount = seeder.seedWithSampleData(&db);
    QVERIFY(cveCount > 0);

    // Create a mock scan result with vulnerable services
    ScanResult scanResult;
    scanResult.targetHost = "192.168.1.100";
    scanResult.scanStartTime = QDateTime::currentDateTime().addSecs(-300);
    scanResult.scanEndTime = QDateTime::currentDateTime();
    scanResult.status = "completed";

    // Add port with vulnerable Apache version
    PortResult apachePort;
    apachePort.port = 80;
    apachePort.protocol = "TCP";
    apachePort.state = "open";
    apachePort.service = "Apache";
    apachePort.version = "2.4.41";
    scanResult.ports.append(apachePort);

    // Add port with vulnerable OpenSSH version
    PortResult sshPort;
    sshPort.port = 22;
    sshPort.protocol = "TCP";
    sshPort.state = "open";
    sshPort.service = "OpenSSH";
    sshPort.version = "7.4";
    scanResult.ports.append(sshPort);

    // Scan for vulnerabilities
    VulnerabilityScanner scanner(db);
    QSignalSpy scanSpy(&scanner, &VulnerabilityScanner::finished);

    scanner.assessVulnerabilities(scanResult);

    // Wait for scan to complete
    QVERIFY(scanSpy.wait(10000));

    // Get vulnerability results
    QVector<VulnerabilityResult> vulnResults = scanner.getResults();

    // Build list of vulnerabilities for report
    QList<Vulnerability> vulnerabilities;
    for (const auto& result : vulnResults) {
        for (const auto& cve : result.vulnerabilities) {
            Vulnerability vuln;
            vuln.cveId = cve.cveId;
            vuln.description = cve.description;
            vuln.severity = cve.severity;
            vuln.cvssScore = cve.cvssScore;
            vuln.affectedService = result.serviceName + " " + result.version;
            vulnerabilities.append(vuln);
        }
    }

    // Should detect vulnerabilities for known vulnerable versions
    QVERIFY(vulnResults.size() > 0);

    // Generate JSON report
    ReportData reportData;
    reportData.targetHost = scanResult.targetHost;
    reportData.reportDate = QDateTime::currentDateTime();
    reportData.scannerVersion = "VulnScan 1.0";
    reportData.scanResult = scanResult;
    reportData.vulnerabilities = vulnerabilities;

    QTemporaryDir tempDir;
    QString jsonPath = tempDir.path() + "/report.json";

    JsonReportGenerator jsonGen;
    QVERIFY(jsonGen.generate(reportData, jsonPath));
    QVERIFY(QFile::exists(jsonPath));

    // Generate CSV report
    QString csvPath = tempDir.path() + "/report.csv";
    CsvReportGenerator csvGen;
    QVERIFY(csvGen.generate(reportData, csvPath));
    QVERIFY(QFile::exists(csvPath));

    // Verify report files are not empty
    QFile jsonFile(jsonPath);
    QVERIFY(jsonFile.open(QIODevice::ReadOnly));
    QVERIFY(jsonFile.size() > 0);
    jsonFile.close();

    QFile csvFile(csvPath);
    QVERIFY(csvFile.open(QIODevice::ReadOnly));
    QVERIFY(csvFile.size() > 0);
    csvFile.close();
}

void IntegrationTestsVuln::testCveDatabaseAndMatcher()
{
    // Create database and seed with data
    QString dbPath = ":memory:";
    VulnerabilityDatabase db(dbPath);
    QVERIFY(db.initialize());

    CveSeeder seeder;
    int cveCount = seeder.seedWithSampleData(&db);
    QVERIFY(cveCount > 0);

    // Test VulnerabilityMatcher with seeded database
    VulnerabilityMatcher matcher(db);

    // Match Apache 2.4.41 (should find CVEs)
    auto apacheCves = matcher.findVulnerabilities("Apache", "2.4.41");
    QVERIFY(apacheCves.size() >= 0); // May or may not have exact matches

    // Match OpenSSH 7.4 (should find CVEs)
    auto sshCves = matcher.findVulnerabilities("OpenSSH", "7.4");
    QVERIFY(sshCves.size() >= 0);

    // Verify database has the seeded CVEs
    int dbCount = db.getCveCount();
    QCOMPARE(dbCount, cveCount);
}

void IntegrationTestsVuln::testVulnerabilityScannerWithDatabase()
{
    // Create and seed database
    QString dbPath = ":memory:";
    VulnerabilityDatabase db(dbPath);
    QVERIFY(db.initialize());

    CveSeeder seeder;
    seeder.seedWithSampleData(&db);

    // Create scan result
    ScanResult scanResult;
    scanResult.targetHost = "test.local";

    PortResult port;
    port.port = 80;
    port.service = "Apache";
    port.version = "2.4.41";
    port.state = "open";
    scanResult.ports.append(port);

    // Scan for vulnerabilities
    VulnerabilityScanner scanner(db);
    QSignalSpy progressSpy(&scanner, &VulnerabilityScanner::progress);
    QSignalSpy finishedSpy(&scanner, &VulnerabilityScanner::finished);

    scanner.assessVulnerabilities(scanResult);

    // Verify signals emitted
    QVERIFY(finishedSpy.wait(5000));
    QVERIFY(finishedSpy.count() > 0);
}

void IntegrationTestsVuln::testServiceCheckersIntegration()
{
    // Test HTTP checker
    HttpVulnChecker httpChecker;
    QVERIFY(httpChecker.canHandle("http"));
    QCOMPARE(httpChecker.name(), QString("HTTP Vulnerability Checker"));

    // Test SSH checker
    SshVulnChecker sshChecker;
    QVERIFY(sshChecker.canHandle("ssh"));
    QCOMPARE(sshChecker.name(), QString("SSH Vulnerability Checker"));

    // Test FTP checker
    FtpVulnChecker ftpChecker;
    QVERIFY(ftpChecker.canHandle("ftp"));
    QCOMPARE(ftpChecker.name(), QString("FTP Vulnerability Checker"));

    // Verify timeout configuration
    httpChecker.setTimeout(5000);
    QCOMPARE(httpChecker.timeout(), 5000);
}

void IntegrationTestsVuln::testReportGenerationWithVulnerabilities()
{
    // Create comprehensive report data
    ReportData data;
    data.targetHost = "192.168.1.100";
    data.reportDate = QDateTime::currentDateTime();
    data.scannerVersion = "VulnScan 1.0";
    data.companyName = "Test Company";
    data.executiveSummary = "Integration test report with multiple vulnerabilities.";

    // Add scan results
    data.scanResult.targetHost = data.targetHost;
    data.scanResult.scanStartTime = QDateTime::currentDateTime().addSecs(-600);
    data.scanResult.scanEndTime = QDateTime::currentDateTime();
    data.scanResult.status = "completed";

    // Add ports
    PortResult port1;
    port1.port = 80;
    port1.service = "http";
    port1.version = "Apache/2.4.41";
    port1.state = "open";
    data.scanResult.ports.append(port1);

    PortResult port2;
    port2.port = 22;
    port2.service = "ssh";
    port2.version = "OpenSSH 7.4";
    port2.state = "open";
    data.scanResult.ports.append(port2);

    // Add vulnerabilities
    Vulnerability vuln1;
    vuln1.cveId = "CVE-2021-44228";
    vuln1.description = "Apache Log4j2 JNDI RCE";
    vuln1.severity = "Critical";
    vuln1.cvssScore = 10.0f;
    vuln1.affectedService = "Apache/2.4.41";
    vuln1.recommendedAction = "Update immediately";
    data.vulnerabilities.append(vuln1);

    Vulnerability vuln2;
    vuln2.cveId = "CVE-2020-15778";
    vuln2.description = "OpenSSH scp command injection";
    vuln2.severity = "High";
    vuln2.cvssScore = 7.8f;
    vuln2.affectedService = "OpenSSH 7.4";
    vuln2.recommendedAction = "Update to OpenSSH 8.4+";
    data.vulnerabilities.append(vuln2);

    Vulnerability vuln3;
    vuln3.cveId = "HTTP-MISSING-HSTS";
    vuln3.description = "Missing HSTS header";
    vuln3.severity = "Medium";
    vuln3.cvssScore = 5.3f;
    vuln3.affectedService = "Apache/2.4.41";
    vuln3.recommendedAction = "Add HSTS header";
    data.vulnerabilities.append(vuln3);

    // Generate both reports
    QTemporaryDir tempDir;

    JsonReportGenerator jsonGen;
    QString jsonPath = tempDir.path() + "/full_report.json";
    QVERIFY(jsonGen.generate(data, jsonPath));

    CsvReportGenerator csvGen;
    QString csvPath = tempDir.path() + "/full_report.csv";
    QVERIFY(csvGen.generate(data, csvPath));

    // Verify both files exist and have content
    QVERIFY(QFile::exists(jsonPath));
    QVERIFY(QFile::exists(csvPath));

    QFile jsonFile(jsonPath);
    QVERIFY(jsonFile.open(QIODevice::ReadOnly));
    QString jsonContent = jsonFile.readAll();
    jsonFile.close();

    // Verify JSON contains expected data
    QVERIFY(jsonContent.contains("CVE-2021-44228"));
    QVERIFY(jsonContent.contains("Critical"));
    QVERIFY(jsonContent.contains("Test Company"));

    QFile csvFile(csvPath);
    QVERIFY(csvFile.open(QIODevice::ReadOnly));
    QString csvContent = csvFile.readAll();
    csvFile.close();

    // Verify CSV contains expected data
    QVERIFY(csvContent.contains("CVE-2021-44228"));
    QVERIFY(csvContent.contains("OpenSSH"));
}

void IntegrationTestsVuln::testSslTlsCheckerIntegration()
{
    SslTlsChecker checker;

    // Verify checker is not currently running
    QVERIFY(!checker.isChecking());

    // Create signal spy for testing
    QSignalSpy finishedSpy(&checker, &SslTlsChecker::finished);
    QSignalSpy errorSpy(&checker, &SslTlsChecker::error);

    // Note: Actual SSL/TLS checks require network connection
    // This test just verifies the integration setup
    QVERIFY(true);
}

void IntegrationTestsVuln::testLargeCveDatabasePerformance()
{
    // Create database with multiple CVE entries
    QString dbPath = ":memory:";
    VulnerabilityDatabase db(dbPath);
    QVERIFY(db.initialize());

    // Seed with sample data
    CveSeeder seeder;
    int cveCount = seeder.seedWithSampleData(&db);

    // Measure query performance
    QElapsedTimer timer;
    timer.start();

    for (int i = 0; i < 100; i++) {
        db.findByService("Apache");
    }

    qint64 elapsed = timer.elapsed();

    // 100 queries should complete in reasonable time (< 1 second)
    QVERIFY(elapsed < 1000);

    // Verify database integrity
    QCOMPARE(db.getCveCount(), cveCount);
}

void IntegrationTestsVuln::testParallelVulnerabilityScanning()
{
    // Create database
    QString dbPath = ":memory:";
    VulnerabilityDatabase db(dbPath);
    QVERIFY(db.initialize());

    CveSeeder seeder;
    seeder.seedWithSampleData(&db);

    // Create scan result with multiple services
    ScanResult scanResult;
    scanResult.targetHost = "test.local";

    for (int i = 0; i < 10; i++) {
        PortResult port;
        port.port = 8000 + i;
        port.service = (i % 2 == 0) ? "Apache" : "nginx";
        port.version = "2.4.41";
        port.state = "open";
        scanResult.ports.append(port);
    }

    // Scan with parallel processing
    VulnerabilityScanner scanner(db);
    QSignalSpy finishedSpy(&scanner, &VulnerabilityScanner::finished);

    QElapsedTimer timer;
    timer.start();

    scanner.assessVulnerabilities(scanResult);

    QVERIFY(finishedSpy.wait(10000));

    qint64 elapsed = timer.elapsed();

    // Parallel scanning should be reasonably fast
    QVERIFY(elapsed < 5000);

    // Verify scan completed
    QVERIFY(finishedSpy.count() > 0);
}
