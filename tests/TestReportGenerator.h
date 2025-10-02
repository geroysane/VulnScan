#ifndef TESTREPORTGENERATOR_H
#define TESTREPORTGENERATOR_H

#include <QObject>
#include <QtTest>
#include "core/report/JsonReportGenerator.h"
#include "core/report/CsvReportGenerator.h"
#include "core/vulnerability/CveSeeder.h"

/**
 * @class TestReportGenerator
 * @brief Unit tests for report generation
 *
 * Tests the JSON and CSV report generators as well as CVE seeding functionality.
 */
class TestReportGenerator : public QObject
{
    Q_OBJECT

private slots:
    // JsonReportGenerator tests
    void testJsonReportGeneratorFormatName();
    void testJsonReportGeneratorEmptyData();
    void testJsonReportGeneratorWithPorts();
    void testJsonReportGeneratorWithVulnerabilities();
    void testJsonReportGeneratorFullData();
    void testJsonReportGeneratorFileCreation();

    // CsvReportGenerator tests
    void testCsvReportGeneratorFormatName();
    void testCsvReportGeneratorEmptyData();
    void testCsvReportGeneratorWithPorts();
    void testCsvReportGeneratorWithVulnerabilities();
    void testCsvReportGeneratorEscaping();
    void testCsvReportGeneratorFileCreation();

    // CveSeeder tests
    void testCveSeederSampleData();
    void testCveSeederDatabaseInsertion();

private:
    /**
     * @brief Create sample report data for testing
     * @return Sample ReportData structure
     */
    ReportData createSampleReportData() const;

    /**
     * @brief Create report data with vulnerabilities
     * @return ReportData with sample vulnerabilities
     */
    ReportData createReportDataWithVulnerabilities() const;
};

#endif // TESTREPORTGENERATOR_H
