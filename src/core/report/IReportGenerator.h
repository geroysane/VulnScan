#ifndef IREPORTGENERATOR_H
#define IREPORTGENERATOR_H

#include <QString>
#include "ReportData.h"

/**
 * @interface IReportGenerator
 * @brief Interface for report generation
 *
 * This interface defines the contract for generating reports
 * in various formats (PDF, HTML, JSON, XML, etc.)
 */
class IReportGenerator {
public:
    virtual ~IReportGenerator() = default;

    /**
     * @brief Generates a report from the provided data
     * @param data The report data to generate from
     * @param outputPath The file path where the report will be saved
     * @return true if generation was successful, false otherwise
     */
    virtual bool generate(const ReportData& data, const QString& outputPath) = 0;

    /**
     * @brief Gets the format name of this generator
     * @return Format name (e.g., "PDF", "HTML", "JSON")
     */
    virtual QString getFormatName() const = 0;
};

#endif // IREPORTGENERATOR_H
