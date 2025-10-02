#include "CsvReportGenerator.h"
#include <QFile>
#include <QTextStream>

CsvReportGenerator::CsvReportGenerator()
{
}

CsvReportGenerator::~CsvReportGenerator()
{
}

QString CsvReportGenerator::getFormatName() const
{
    return "CSV";
}

bool CsvReportGenerator::generate(const ReportData& data, const QString& outputPath)
{
    QString csvContent;

    // Generate vulnerabilities CSV if any exist
    if (!data.vulnerabilities.isEmpty()) {
        csvContent = createVulnerabilitiesCsv(data);
    } else {
        // Fallback to ports/services CSV
        csvContent = createPortsCsv(data);
    }

    // Write to file
    QFile file(outputPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream out(&file);
    out << csvContent;
    file.close();

    return true;
}

QString CsvReportGenerator::escapeCsvField(const QString& field) const
{
    // If field contains comma, quote, or newline, wrap in quotes and escape quotes
    if (field.contains(',') || field.contains('"') || field.contains('\n')) {
        QString escaped = field;
        escaped.replace("\"", "\"\""); // Escape quotes by doubling them
        return QString("\"%1\"").arg(escaped);
    }
    return field;
}

QString CsvReportGenerator::createCsvRow(const QStringList& values) const
{
    QStringList escapedValues;
    for (const QString& value : values) {
        escapedValues.append(escapeCsvField(value));
    }
    return escapedValues.join(",") + "\n";
}

QString CsvReportGenerator::createVulnerabilitiesCsv(const ReportData& data) const
{
    QString csv;

    // Header row
    QStringList headers;
    headers << "Host" << "Port" << "Service" << "Version" << "CVE ID"
            << "Severity" << "CVSS Score" << "Description" << "Recommended Action";
    csv += createCsvRow(headers);

    // Data rows - one per vulnerability
    for (const Vulnerability& vuln : data.vulnerabilities) {
        // Try to find matching port/service for this vulnerability
        QString port;
        QString service;
        QString version;

        // Match vulnerability to service by parsing affectedService
        // Format is typically "ServiceName Version" or just "ServiceName"
        for (const PortResult& portResult : data.scanResult.ports) {
            if (vuln.affectedService.contains(portResult.service, Qt::CaseInsensitive)) {
                port = QString::number(portResult.port);
                service = portResult.service;
                version = portResult.version;
                break;
            }
        }

        // If no match found, use affected service as-is
        if (service.isEmpty()) {
            service = vuln.affectedService;
        }

        QStringList row;
        row << data.targetHost
            << port
            << service
            << version
            << vuln.cveId
            << vuln.severity
            << QString::number(vuln.cvssScore, 'f', 1)
            << vuln.description
            << vuln.recommendedAction;

        csv += createCsvRow(row);
    }

    return csv;
}

QString CsvReportGenerator::createPortsCsv(const ReportData& data) const
{
    QString csv;

    // Header row
    QStringList headers;
    headers << "Host" << "Port" << "Protocol" << "State" << "Service" << "Version";
    csv += createCsvRow(headers);

    // Data rows - one per port
    for (const PortResult& port : data.scanResult.ports) {
        QStringList row;
        row << data.targetHost
            << QString::number(port.port)
            << port.protocol
            << port.state
            << port.service
            << port.version;

        csv += createCsvRow(row);
    }

    return csv;
}
