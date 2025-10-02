#include "TestReportGenerator.h"
#include "core/vulnerability/VulnerabilityDatabase.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTemporaryDir>

// JsonReportGenerator tests
void TestReportGenerator::testJsonReportGeneratorFormatName()
{
    JsonReportGenerator generator;
    QCOMPARE(generator.getFormatName(), QString("JSON"));
}

void TestReportGenerator::testJsonReportGeneratorEmptyData()
{
    JsonReportGenerator generator;
    ReportData data;
    data.targetHost = "192.168.1.1";
    data.reportDate = QDateTime::currentDateTime();
    data.scannerVersion = "VulnScan 1.0";

    QTemporaryDir tempDir;
    QVERIFY(tempDir.isValid());

    QString outputPath = tempDir.path() + "/report.json";
    QVERIFY(generator.generate(data, outputPath));

    // Verify file exists
    QVERIFY(QFile::exists(outputPath));

    // Read and verify JSON structure
    QFile file(outputPath);
    QVERIFY(file.open(QIODevice::ReadOnly));
    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    QVERIFY(!doc.isNull());
    QVERIFY(doc.isObject());

    QJsonObject root = doc.object();
    QVERIFY(root.contains("metadata"));
    QVERIFY(root.contains("summary"));
    QVERIFY(root.contains("ports"));
    QVERIFY(root.contains("vulnerabilities"));
}

void TestReportGenerator::testJsonReportGeneratorWithPorts()
{
    JsonReportGenerator generator;
    ReportData data = createSampleReportData();

    QTemporaryDir tempDir;
    QString outputPath = tempDir.path() + "/report.json";
    QVERIFY(generator.generate(data, outputPath));

    // Read JSON
    QFile file(outputPath);
    QVERIFY(file.open(QIODevice::ReadOnly));
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    QJsonObject root = doc.object();
    QJsonArray ports = root["ports"].toArray();
    QCOMPARE(ports.size(), data.scanResult.ports.size());

    // Verify port structure
    if (ports.size() > 0) {
        QJsonObject port = ports[0].toObject();
        QVERIFY(port.contains("port"));
        QVERIFY(port.contains("protocol"));
        QVERIFY(port.contains("state"));
        QVERIFY(port.contains("service"));
    }
}

void TestReportGenerator::testJsonReportGeneratorWithVulnerabilities()
{
    JsonReportGenerator generator;
    ReportData data = createReportDataWithVulnerabilities();

    QTemporaryDir tempDir;
    QString outputPath = tempDir.path() + "/report.json";
    QVERIFY(generator.generate(data, outputPath));

    // Read JSON
    QFile file(outputPath);
    QVERIFY(file.open(QIODevice::ReadOnly));
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    QJsonObject root = doc.object();
    QJsonArray vulns = root["vulnerabilities"].toArray();
    QCOMPARE(vulns.size(), data.vulnerabilities.size());

    // Verify vulnerability structure
    if (vulns.size() > 0) {
        QJsonObject vuln = vulns[0].toObject();
        QVERIFY(vuln.contains("cve_id"));
        QVERIFY(vuln.contains("description"));
        QVERIFY(vuln.contains("severity"));
        QVERIFY(vuln.contains("cvss_score"));
    }

    // Verify severity summary
    QJsonObject severitySummary = root["severity_summary"].toObject();
    QVERIFY(severitySummary.contains("critical"));
    QVERIFY(severitySummary.contains("high"));
    QVERIFY(severitySummary.contains("medium"));
    QVERIFY(severitySummary.contains("low"));
    QVERIFY(severitySummary.contains("total"));
}

void TestReportGenerator::testJsonReportGeneratorFullData()
{
    JsonReportGenerator generator;
    ReportData data = createReportDataWithVulnerabilities();
    data.executiveSummary = "This is a test executive summary.";
    data.companyName = "Test Company Inc.";

    QTemporaryDir tempDir;
    QString outputPath = tempDir.path() + "/report.json";
    QVERIFY(generator.generate(data, outputPath));

    // Read JSON
    QFile file(outputPath);
    QVERIFY(file.open(QIODevice::ReadOnly));
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    QJsonObject root = doc.object();

    // Verify executive summary
    QVERIFY(root.contains("executive_summary"));
    QCOMPARE(root["executive_summary"].toString(), data.executiveSummary);

    // Verify metadata contains company name
    QJsonObject metadata = root["metadata"].toObject();
    QVERIFY(metadata.contains("company_name"));
    QCOMPARE(metadata["company_name"].toString(), data.companyName);
}

void TestReportGenerator::testJsonReportGeneratorFileCreation()
{
    JsonReportGenerator generator;
    ReportData data = createSampleReportData();

    QTemporaryDir tempDir;
    QString outputPath = tempDir.path() + "/test_output.json";

    QVERIFY(generator.generate(data, outputPath));
    QVERIFY(QFile::exists(outputPath));

    // Verify file is not empty
    QFile file(outputPath);
    QVERIFY(file.open(QIODevice::ReadOnly));
    QVERIFY(file.size() > 0);
    file.close();
}

// CsvReportGenerator tests
void TestReportGenerator::testCsvReportGeneratorFormatName()
{
    CsvReportGenerator generator;
    QCOMPARE(generator.getFormatName(), QString("CSV"));
}

void TestReportGenerator::testCsvReportGeneratorEmptyData()
{
    CsvReportGenerator generator;
    ReportData data;
    data.targetHost = "192.168.1.1";

    QTemporaryDir tempDir;
    QString outputPath = tempDir.path() + "/report.csv";
    QVERIFY(generator.generate(data, outputPath));

    // Verify file exists
    QVERIFY(QFile::exists(outputPath));

    // Read CSV
    QFile file(outputPath);
    QVERIFY(file.open(QIODevice::ReadOnly | QIODevice::Text));
    QString csvContent = file.readAll();
    file.close();

    // Should have at least header row
    QVERIFY(csvContent.contains("Host"));
}

void TestReportGenerator::testCsvReportGeneratorWithPorts()
{
    CsvReportGenerator generator;
    ReportData data = createSampleReportData();

    QTemporaryDir tempDir;
    QString outputPath = tempDir.path() + "/report.csv";
    QVERIFY(generator.generate(data, outputPath));

    // Read CSV
    QFile file(outputPath);
    QVERIFY(file.open(QIODevice::ReadOnly | QIODevice::Text));
    QStringList lines = QString(file.readAll()).split('\n', Qt::SkipEmptyParts);
    file.close();

    // Should have header + data rows
    QVERIFY(lines.size() > 1);

    // Verify header contains expected columns
    QString header = lines[0];
    QVERIFY(header.contains("Host"));
    QVERIFY(header.contains("Port"));
    QVERIFY(header.contains("Service"));
}

void TestReportGenerator::testCsvReportGeneratorWithVulnerabilities()
{
    CsvReportGenerator generator;
    ReportData data = createReportDataWithVulnerabilities();

    QTemporaryDir tempDir;
    QString outputPath = tempDir.path() + "/report.csv";
    QVERIFY(generator.generate(data, outputPath));

    // Read CSV
    QFile file(outputPath);
    QVERIFY(file.open(QIODevice::ReadOnly | QIODevice::Text));
    QStringList lines = QString(file.readAll()).split('\n', Qt::SkipEmptyParts);
    file.close();

    // Should have header + vulnerability rows
    QVERIFY(lines.size() > 1);

    // Verify header contains vulnerability columns
    QString header = lines[0];
    QVERIFY(header.contains("CVE ID"));
    QVERIFY(header.contains("Severity"));
    QVERIFY(header.contains("Description"));
}

void TestReportGenerator::testCsvReportGeneratorEscaping()
{
    CsvReportGenerator generator;
    ReportData data;
    data.targetHost = "192.168.1.1";

    // Add vulnerability with special characters
    Vulnerability vuln;
    vuln.cveId = "CVE-2021-TEST";
    vuln.description = "This description has a comma, and \"quotes\", and\nnewlines.";
    vuln.severity = "High";
    vuln.cvssScore = 7.5f;
    vuln.affectedService = "Test Service";
    vuln.recommendedAction = "Update software";
    data.vulnerabilities.append(vuln);

    QTemporaryDir tempDir;
    QString outputPath = tempDir.path() + "/report.csv";
    QVERIFY(generator.generate(data, outputPath));

    // Read CSV
    QFile file(outputPath);
    QVERIFY(file.open(QIODevice::ReadOnly | QIODevice::Text));
    QString csvContent = file.readAll();
    file.close();

    // Verify escaping (field with comma/quotes should be wrapped in quotes)
    QVERIFY(csvContent.contains("\"This description has a comma"));
}

void TestReportGenerator::testCsvReportGeneratorFileCreation()
{
    CsvReportGenerator generator;
    ReportData data = createSampleReportData();

    QTemporaryDir tempDir;
    QString outputPath = tempDir.path() + "/test_output.csv";

    QVERIFY(generator.generate(data, outputPath));
    QVERIFY(QFile::exists(outputPath));

    // Verify file is not empty
    QFile file(outputPath);
    QVERIFY(file.open(QIODevice::ReadOnly));
    QVERIFY(file.size() > 0);
    file.close();
}

// CveSeeder tests
void TestReportGenerator::testCveSeederSampleData()
{
    QList<CveEntry> sampleData = CveSeeder::getSampleCveData();

    // Should have multiple sample CVEs
    QVERIFY(sampleData.size() >= 5);

    // Verify structure of first entry
    if (sampleData.size() > 0) {
        CveEntry entry = sampleData[0];
        QVERIFY(!entry.cveId.isEmpty());
        QVERIFY(!entry.description.isEmpty());
        QVERIFY(!entry.severity.isEmpty());
        QVERIFY(entry.cvssScore > 0.0f);
        QVERIFY(!entry.affectedService.isEmpty());
    }
}

void TestReportGenerator::testCveSeederDatabaseInsertion()
{
    // Create temporary database
    QString dbPath = ":memory:"; // In-memory SQLite database
    VulnerabilityDatabase db(dbPath);

    // Initialize database
    QVERIFY(db.initialize());

    // Seed with sample data
    CveSeeder seeder;
    int count = seeder.seedWithSampleData(&db);

    // Verify CVEs were inserted
    QVERIFY(count > 0);

    // Verify we can retrieve CVEs
    int dbCount = db.getCveCount();
    QCOMPARE(dbCount, count);

    // Try finding a specific CVE
    auto cves = db.getAllCves();
    QCOMPARE(cves.size(), count);
}

// Helper methods
ReportData TestReportGenerator::createSampleReportData() const
{
    ReportData data;
    data.targetHost = "192.168.1.100";
    data.reportDate = QDateTime::currentDateTime();
    data.scannerVersion = "VulnScan 1.0";
    data.scanResult.targetHost = "192.168.1.100";
    data.scanResult.scanStartTime = QDateTime::currentDateTime().addSecs(-300);
    data.scanResult.scanEndTime = QDateTime::currentDateTime();
    data.scanResult.status = "completed";

    // Add some ports
    PortResult port1;
    port1.port = 80;
    port1.protocol = "TCP";
    port1.state = "open";
    port1.service = "http";
    port1.version = "Apache 2.4.41";
    data.scanResult.ports.append(port1);

    PortResult port2;
    port2.port = 22;
    port2.protocol = "TCP";
    port2.state = "open";
    port2.service = "ssh";
    port2.version = "OpenSSH 7.4";
    data.scanResult.ports.append(port2);

    return data;
}

ReportData TestReportGenerator::createReportDataWithVulnerabilities() const
{
    ReportData data = createSampleReportData();

    // Add vulnerabilities
    Vulnerability vuln1;
    vuln1.cveId = "CVE-2021-44228";
    vuln1.description = "Apache Log4j2 JNDI RCE vulnerability";
    vuln1.severity = "Critical";
    vuln1.cvssScore = 10.0f;
    vuln1.affectedService = "Apache 2.4.41";
    vuln1.recommendedAction = "Update to Apache 2.4.51 or later";
    data.vulnerabilities.append(vuln1);

    Vulnerability vuln2;
    vuln2.cveId = "CVE-2020-15778";
    vuln2.description = "OpenSSH scp command injection";
    vuln2.severity = "High";
    vuln2.cvssScore = 7.8f;
    vuln2.affectedService = "OpenSSH 7.4";
    vuln2.recommendedAction = "Update to OpenSSH 8.4 or later";
    data.vulnerabilities.append(vuln2);

    Vulnerability vuln3;
    vuln3.cveId = "HTTP-MISSING-HSTS";
    vuln3.description = "Missing HSTS header";
    vuln3.severity = "Medium";
    vuln3.cvssScore = 5.3f;
    vuln3.affectedService = "Apache 2.4.41";
    vuln3.recommendedAction = "Add Strict-Transport-Security header";
    data.vulnerabilities.append(vuln3);

    return data;
}
