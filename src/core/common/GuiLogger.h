#ifndef GUILOGGER_H
#define GUILOGGER_H

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QMutex>
#include <QFile>
#include <QTextStream>

/**
 * @class GuiLogger
 * @brief GUI-safe logging system without qInstallMessageHandler
 *
 * This logger is designed specifically for GUI applications to avoid
 * deadlocks caused by qInstallMessageHandler recursion with Qt's
 * internal message handling.
 *
 * Features:
 * - Signal-based log message distribution
 * - Thread-safe logging with QMutex
 * - File logging support
 * - Multiple log levels (Debug, Info, Warning, Error, Critical)
 * - No qInstallMessageHandler usage
 *
 * Usage in GUI:
 * @code
 * GuiLogger* logger = GuiLogger::instance();
 * connect(logger, &GuiLogger::logMessage, this, &MainWindow::onLogMessage);
 * logger->info("Application started");
 * @endcode
 */
class GuiLogger : public QObject
{
    Q_OBJECT

public:
    /**
     * @enum LogLevel
     * @brief Log severity levels
     */
    enum LogLevel {
        Debug,
        Info,
        Warning,
        Error,
        Critical
    };
    Q_ENUM(LogLevel)

    /**
     * @brief Get singleton instance
     * @return GuiLogger singleton
     */
    static GuiLogger* instance();

    /**
     * @brief Enable or disable file logging
     * @param enabled true to enable file logging
     * @param filePath Path to log file
     * @return true if successful
     */
    bool setFileLogging(bool enabled, const QString& filePath = QString());

    /**
     * @brief Set minimum log level
     * @param level Minimum level to log
     */
    void setLogLevel(LogLevel level);

    /**
     * @brief Log debug message
     * @param message Message to log
     * @param context Optional context/source
     */
    void debug(const QString& message, const QString& context = QString());

    /**
     * @brief Log info message
     * @param message Message to log
     * @param context Optional context/source
     */
    void info(const QString& message, const QString& context = QString());

    /**
     * @brief Log warning message
     * @param message Message to log
     * @param context Optional context/source
     */
    void warning(const QString& message, const QString& context = QString());

    /**
     * @brief Log error message
     * @param message Message to log
     * @param context Optional context/source
     */
    void error(const QString& message, const QString& context = QString());

    /**
     * @brief Log critical message
     * @param message Message to log
     * @param context Optional context/source
     */
    void critical(const QString& message, const QString& context = QString());

signals:
    /**
     * @brief Emitted when a log message is generated
     * @param level Log level
     * @param timestamp Message timestamp
     * @param context Message context/source
     * @param message Log message
     */
    void logMessage(GuiLogger::LogLevel level, const QDateTime& timestamp,
                   const QString& context, const QString& message);

private:
    /**
     * @brief Private constructor for singleton
     */
    explicit GuiLogger(QObject* parent = nullptr);

    /**
     * @brief Destructor
     */
    ~GuiLogger();

    // Prevent copying
    GuiLogger(const GuiLogger&) = delete;
    GuiLogger& operator=(const GuiLogger&) = delete;

    /**
     * @brief Internal logging function
     * @param level Log level
     * @param message Log message
     * @param context Optional context
     */
    void log(LogLevel level, const QString& message, const QString& context);

    /**
     * @brief Convert log level to string
     * @param level Log level
     * @return String representation
     */
    QString levelToString(LogLevel level) const;

    /**
     * @brief Write to log file
     * @param level Log level
     * @param timestamp Timestamp
     * @param context Context
     * @param message Message
     */
    void writeToFile(LogLevel level, const QDateTime& timestamp,
                    const QString& context, const QString& message);

    static GuiLogger* s_instance; ///< Singleton instance
    QMutex m_mutex;              ///< Thread safety mutex
    LogLevel m_minLevel;         ///< Minimum log level
    bool m_fileLoggingEnabled;   ///< File logging flag
    QString m_logFilePath;       ///< Log file path
    QFile* m_logFile;            ///< Log file handle
    QTextStream* m_logStream;    ///< Log text stream
};

#endif // GUILOGGER_H
