#ifndef REPORTDATA_H
#define REPORTDATA_H

#include <QString>
#include <QDateTime>
#include <QList>
#include <QMetaType>
#include "scanner/ScanResult.h"
#include "scanner/Vulnerability.h"
#include "ai/AIAnalysis.h"

/**
 * @struct ReportData
 * @brief Contains all data needed for report generation
 *
 * This structure aggregates scan results, vulnerabilities,
 * and AI analysis for comprehensive report generation.
 */
struct ReportData {
    QString targetHost;                  ///< Target hostname or IP
    QDateTime reportDate;                ///< Report generation date
    ScanResult scanResult;               ///< Complete scan results
    QList<Vulnerability> vulnerabilities; ///< All detected vulnerabilities
    AIAnalysis aiAnalysis;               ///< AI-based analysis
    QString executiveSummary;            ///< Executive summary section
    QString companyName;                 ///< Company name (optional)
    QString scannerVersion;              ///< VulnScan version used

    ReportData() {}
};

Q_DECLARE_METATYPE(ReportData)

#endif // REPORTDATA_H
