#include "JsonReportGenerator.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

JsonReportGenerator::JsonReportGenerator()
{
}

JsonReportGenerator::~JsonReportGenerator()
{
}

QString JsonReportGenerator::getFormatName() const
{
    return "JSON";
}

bool JsonReportGenerator::generate(const ReportData& data, const QString& outputPath)
{
    // Create main JSON object
    QJsonObject root;

    // Add sections
    root["metadata"] = createMetadata(data);
    root["summary"] = createSummary(data);
    root["ports"] = createPorts(data.scanResult);
    root["vulnerabilities"] = createVulnerabilities(data.vulnerabilities);
    root["severity_summary"] = createSeveritySummary(data.vulnerabilities);

    // Add executive summary if present
    if (!data.executiveSummary.isEmpty()) {
        root["executive_summary"] = data.executiveSummary;
    }

    // Convert to JSON document
    QJsonDocument doc(root);

    // Write to file
    QFile file(outputPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

    return true;
}

QJsonObject JsonReportGenerator::createMetadata(const ReportData& data) const
{
    QJsonObject metadata;

    metadata["target_host"] = data.targetHost;
    metadata["report_date"] = data.reportDate.toString(Qt::ISODate);
    metadata["scanner_version"] = data.scannerVersion.isEmpty() ? "VulnScan 1.0" : data.scannerVersion;

    if (!data.companyName.isEmpty()) {
        metadata["company_name"] = data.companyName;
    }

    // Scan timing information
    if (!data.scanResult.scanStartTime.isNull()) {
        metadata["scan_start_time"] = data.scanResult.scanStartTime.toString(Qt::ISODate);
    }
    if (!data.scanResult.scanEndTime.isNull()) {
        metadata["scan_end_time"] = data.scanResult.scanEndTime.toString(Qt::ISODate);

        // Calculate scan duration
        if (!data.scanResult.scanStartTime.isNull()) {
            qint64 durationSeconds = data.scanResult.scanStartTime.secsTo(data.scanResult.scanEndTime);
            metadata["scan_duration_seconds"] = durationSeconds;
        }
    }

    metadata["scan_status"] = data.scanResult.status;

    return metadata;
}

QJsonObject JsonReportGenerator::createSummary(const ReportData& data) const
{
    QJsonObject summary;

    // Count open ports
    int openPorts = 0;
    int closedPorts = 0;
    int filteredPorts = 0;
    QSet<QString> uniqueServices;

    for (const PortResult& port : data.scanResult.ports) {
        if (port.state == "open") {
            openPorts++;
            if (!port.service.isEmpty()) {
                uniqueServices.insert(port.service);
            }
        } else if (port.state == "closed") {
            closedPorts++;
        } else if (port.state == "filtered") {
            filteredPorts++;
        }
    }

    summary["total_ports_scanned"] = data.scanResult.ports.size();
    summary["open_ports"] = openPorts;
    summary["closed_ports"] = closedPorts;
    summary["filtered_ports"] = filteredPorts;
    summary["unique_services"] = uniqueServices.size();
    summary["total_vulnerabilities"] = data.vulnerabilities.size();

    // Vulnerability severity breakdown
    summary["critical_vulnerabilities"] = countBySeverity(data.vulnerabilities, "Critical");
    summary["high_vulnerabilities"] = countBySeverity(data.vulnerabilities, "High");
    summary["medium_vulnerabilities"] = countBySeverity(data.vulnerabilities, "Medium");
    summary["low_vulnerabilities"] = countBySeverity(data.vulnerabilities, "Low");

    return summary;
}

QJsonArray JsonReportGenerator::createPorts(const ScanResult& scanResult) const
{
    QJsonArray portsArray;

    for (const PortResult& port : scanResult.ports) {
        QJsonObject portObj;

        portObj["port"] = port.port;
        portObj["protocol"] = port.protocol;
        portObj["state"] = port.state;
        portObj["service"] = port.service;
        portObj["version"] = port.version;

        portsArray.append(portObj);
    }

    return portsArray;
}

QJsonArray JsonReportGenerator::createVulnerabilities(const QList<Vulnerability>& vulnerabilities) const
{
    QJsonArray vulnArray;

    for (const Vulnerability& vuln : vulnerabilities) {
        QJsonObject vulnObj;

        vulnObj["cve_id"] = vuln.cveId;
        vulnObj["description"] = vuln.description;
        vulnObj["severity"] = vuln.severity;
        vulnObj["cvss_score"] = vuln.cvssScore;
        vulnObj["affected_service"] = vuln.affectedService;
        vulnObj["recommended_action"] = vuln.recommendedAction;

        vulnArray.append(vulnObj);
    }

    return vulnArray;
}

QJsonObject JsonReportGenerator::createSeveritySummary(const QList<Vulnerability>& vulnerabilities) const
{
    QJsonObject summary;

    int critical = countBySeverity(vulnerabilities, "Critical");
    int high = countBySeverity(vulnerabilities, "High");
    int medium = countBySeverity(vulnerabilities, "Medium");
    int low = countBySeverity(vulnerabilities, "Low");

    summary["critical"] = critical;
    summary["high"] = high;
    summary["medium"] = medium;
    summary["low"] = low;
    summary["total"] = vulnerabilities.size();

    // Calculate risk score (weighted average)
    // Critical: 10, High: 7, Medium: 4, Low: 1
    double riskScore = 0.0;
    if (vulnerabilities.size() > 0) {
        riskScore = (critical * 10.0 + high * 7.0 + medium * 4.0 + low * 1.0) / vulnerabilities.size();
    }
    summary["risk_score"] = riskScore;

    return summary;
}

int JsonReportGenerator::countBySeverity(const QList<Vulnerability>& vulnerabilities, const QString& severity) const
{
    int count = 0;
    for (const Vulnerability& vuln : vulnerabilities) {
        if (vuln.severity.compare(severity, Qt::CaseInsensitive) == 0) {
            count++;
        }
    }
    return count;
}
