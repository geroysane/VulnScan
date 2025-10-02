#ifndef JSONREPORTGENERATOR_H
#define JSONREPORTGENERATOR_H

#include "IReportGenerator.h"
#include <QJsonObject>
#include <QJsonArray>

/**
 * @class JsonReportGenerator
 * @brief Generates reports in JSON format
 *
 * Creates structured JSON reports containing scan results,
 * vulnerabilities, severity summaries, and metadata.
 *
 * JSON format includes:
 * - Scan metadata (target, date, version)
 * - Summary statistics (ports, services, vulnerabilities)
 * - Detailed port and service information
 * - Vulnerability details with severity categorization
 * - Executive summary
 */
class JsonReportGenerator : public IReportGenerator
{
public:
    /**
     * @brief Constructor
     */
    JsonReportGenerator();

    /**
     * @brief Destructor
     */
    ~JsonReportGenerator() override;

    /**
     * @brief Generate JSON report
     * @param data Report data to export
     * @param outputPath File path for JSON output
     * @return true if generation successful
     */
    bool generate(const ReportData& data, const QString& outputPath) override;

    /**
     * @brief Get format name
     * @return "JSON"
     */
    QString getFormatName() const override;

private:
    /**
     * @brief Create metadata section
     * @param data Report data
     * @return JSON object with metadata
     */
    QJsonObject createMetadata(const ReportData& data) const;

    /**
     * @brief Create summary section
     * @param data Report data
     * @return JSON object with summary statistics
     */
    QJsonObject createSummary(const ReportData& data) const;

    /**
     * @brief Create ports section
     * @param scanResult Scan result with ports
     * @return JSON array of port details
     */
    QJsonArray createPorts(const ScanResult& scanResult) const;

    /**
     * @brief Create vulnerabilities section
     * @param vulnerabilities List of vulnerabilities
     * @return JSON array of vulnerability details
     */
    QJsonArray createVulnerabilities(const QList<Vulnerability>& vulnerabilities) const;

    /**
     * @brief Create severity summary
     * @param vulnerabilities List of vulnerabilities
     * @return JSON object with severity counts
     */
    QJsonObject createSeveritySummary(const QList<Vulnerability>& vulnerabilities) const;

    /**
     * @brief Count vulnerabilities by severity
     * @param vulnerabilities List of vulnerabilities
     * @param severity Severity level to count
     * @return Count of vulnerabilities with specified severity
     */
    int countBySeverity(const QList<Vulnerability>& vulnerabilities, const QString& severity) const;
};

#endif // JSONREPORTGENERATOR_H
