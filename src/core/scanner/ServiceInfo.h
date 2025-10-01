#ifndef SERVICEINFO_H
#define SERVICEINFO_H

#include <QString>
#include <QMetaType>

/**
 * @struct ServiceInfo
 * @brief Information about a detected service
 *
 * Contains detailed information about a network service
 * detected during port scanning operations.
 */
struct ServiceInfo {
    QString name;          ///< Service name (e.g., "http", "ssh")
    QString version;       ///< Service version string
    int port;              ///< Port number where service is running
    QString protocol;      ///< Protocol type (TCP, UDP)
    QString banner;        ///< Service banner information

    ServiceInfo() : port(0) {}
};

Q_DECLARE_METATYPE(ServiceInfo)

#endif // SERVICEINFO_H
