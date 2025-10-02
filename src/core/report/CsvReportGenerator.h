#ifndef CSVREPORTGENERATOR_H
#define CSVREPORTGENERATOR_H

#include "IReportGenerator.h"

/**
 * @class CsvReportGenerator
 * @brief Generates reports in CSV format
 *
 * Creates comma-separated values (CSV) reports suitable for
 * spreadsheet applications (Excel, LibreOffice Calc, etc.).
 *
 * CSV format includes:
 * - Header row with column names
 * - One row per vulnerability
 * - Columns: Host, Port, Service, Version, CVE ID, Severity, CVSS Score, Description, Recommendation
 *
 * For scans without vulnerabilities, includes port/service information.
 */
class CsvReportGenerator : public IReportGenerator
{
public:
    /**
     * @brief Constructor
     */
    CsvReportGenerator();

    /**
     * @brief Destructor
     */
    ~CsvReportGenerator() override;

    /**
     * @brief Generate CSV report
     * @param data Report data to export
     * @param outputPath File path for CSV output
     * @return true if generation successful
     */
    bool generate(const ReportData& data, const QString& outputPath) override;

    /**
     * @brief Get format name
     * @return "CSV"
     */
    QString getFormatName() const override;

private:
    /**
     * @brief Escape CSV field (handle commas, quotes, newlines)
     * @param field Field value to escape
     * @return Escaped field value
     */
    QString escapeCsvField(const QString& field) const;

    /**
     * @brief Create CSV row from values
     * @param values List of field values
     * @return CSV row string
     */
    QString createCsvRow(const QStringList& values) const;

    /**
     * @brief Write vulnerabilities to CSV
     * @param data Report data
     * @return CSV content string
     */
    QString createVulnerabilitiesCsv(const ReportData& data) const;

    /**
     * @brief Write ports/services to CSV (fallback if no vulnerabilities)
     * @param data Report data
     * @return CSV content string
     */
    QString createPortsCsv(const ReportData& data) const;
};

#endif // CSVREPORTGENERATOR_H
