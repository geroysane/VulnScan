#ifndef SCANRESULT_H
#define SCANRESULT_H

#include <QString>
#include <QDateTime>
#include <QList>
#include <QMetaType>
#include "PortResult.h"

/**
 * @struct ScanResult
 * @brief Contains the results of a network scan
 *
 * This structure encapsulates all information about a completed
 * or ongoing network scan including target, timing, and discovered ports.
 */
struct ScanResult {
    int id;                      ///< Unique scan identifier
    QString targetHost;          ///< Target hostname or IP address
    QDateTime scanStartTime;     ///< Scan start timestamp
    QDateTime scanEndTime;       ///< Scan completion timestamp
    QList<PortResult> ports;     ///< List of scanned ports with results
    QString status;              ///< Scan status (running, completed, failed)

    ScanResult() : id(0) {}
};

Q_DECLARE_METATYPE(ScanResult)

#endif // SCANRESULT_H
