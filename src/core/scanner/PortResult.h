#ifndef PORTRESULT_H
#define PORTRESULT_H

#include <QString>
#include <QMetaType>

/**
 * @struct PortResult
 * @brief Result of a single port scan
 *
 * Contains information about a scanned port including its state,
 * detected service, and version information.
 */
struct PortResult {
    int port;              ///< Port number
    QString protocol;      ///< Protocol type (TCP, UDP)
    QString state;         ///< Port state (open, closed, filtered)
    QString service;       ///< Detected service name
    QString version;       ///< Service version string

    PortResult() : port(0) {}
};

Q_DECLARE_METATYPE(PortResult)

#endif // PORTRESULT_H
