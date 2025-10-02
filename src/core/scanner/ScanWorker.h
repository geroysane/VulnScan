#ifndef SCANWORKER_H
#define SCANWORKER_H

#include "PortResult.h"
#include <QRunnable>
#include <QObject>
#include <QString>
#include <QAtomicInt>

/**
 * @class ScanWorker
 * @brief Thread worker for scanning a single port
 *
 * This class implements a QRunnable worker that performs a TCP connect scan
 * on a single port. It is designed to be used with QThreadPool for parallel
 * port scanning.
 *
 * Features:
 * - TCP connect() scan
 * - Configurable timeout
 * - Thread-safe operation
 * - Cancellation support
 * - Result emission via signal
 *
 * The worker performs a simple TCP connection attempt to determine if a port
 * is open, closed, or filtered. It does not perform service detection (that's
 * handled by ServiceDetector in a later phase).
 */
class ScanWorker : public QObject, public QRunnable {
    Q_OBJECT

public:
    /**
     * @brief Constructor
     * @param target Target host (IP or hostname)
     * @param port Port number to scan
     * @param timeout Connection timeout in milliseconds
     * @param cancelFlag Shared atomic flag for cancellation
     */
    ScanWorker(const QString &target,
               int port,
               int timeout,
               QAtomicInt *cancelFlag);

    /**
     * @brief Destructor
     */
    ~ScanWorker() override;

    /**
     * @brief Main worker execution (called by thread pool)
     *
     * Performs the actual port scan and emits the result.
     * This method is called automatically by QThreadPool.
     */
    void run() override;

signals:
    /**
     * @brief Emitted when port scan completes
     * @param result PortResult containing scan outcome
     */
    void scanCompleted(const PortResult &result);

private:
    QString m_target;          ///< Target host
    int m_port;                ///< Port to scan
    int m_timeout;             ///< Connection timeout (ms)
    QAtomicInt *m_cancelFlag;  ///< Shared cancellation flag

    /**
     * @brief Performs TCP connect scan on the port
     * @return PortResult with scan outcome
     */
    PortResult scanPort();

    /**
     * @brief Determines port state from socket error
     * @param errorString Qt socket error description
     * @return State string ("open", "closed", "filtered")
     */
    QString getStateFromError(const QString &errorString) const;
};

#endif // SCANWORKER_H
