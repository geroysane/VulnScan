#ifndef LOGGER_H
#define LOGGER_H

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QMutex>

/**
 * @class Logger
 * @brief Application-wide logging system
 *
 * This class provides a centralized logging mechanism for the entire
 * application. It supports multiple log levels and can log to both
 * file and console simultaneously.
 */
class Logger {
public:
    /**
     * @enum Level
     * @brief Log level enumeration
     */
    enum class Level {
        Debug,      ///< Debug messages (most verbose)
        Info,       ///< Informational messages
        Warning,    ///< Warning messages
        Error,      ///< Error messages
        Critical    ///< Critical error messages (least verbose)
    };

    /**
     * @brief Initializes the logging system
     * @param logFilePath Path to the log file
     * @param enableConsole Enable console output
     */
    static void initialize(const QString& logFilePath, bool enableConsole = true);

    /**
     * @brief Logs a message with specified level
     * @param level Log level
     * @param message Message to log
     */
    static void log(Level level, const QString& message);

    /**
     * @brief Sets the minimum log level
     * @param level Minimum level to log
     */
    static void setLogLevel(Level level);

    /**
     * @brief Closes the log file
     */
    static void shutdown();

    // Convenience methods
    static void debug(const QString& message);
    static void info(const QString& message);
    static void warning(const QString& message);
    static void error(const QString& message);
    static void critical(const QString& message);

private:
    /**
     * @brief Custom Qt message handler
     * @param type Message type from Qt
     * @param context Message context
     * @param msg Message string
     */
    static void messageHandler(QtMsgType type,
                              const QMessageLogContext& context,
                              const QString& msg);

    /**
     * @brief Converts log level to string
     * @param level Log level
     * @return String representation
     */
    static QString levelToString(Level level);

    /**
     * @brief Converts QtMsgType to Logger::Level
     * @param type Qt message type
     * @return Corresponding Logger::Level
     */
    static Level qtMsgTypeToLevel(QtMsgType type);

    static QFile* s_logFile;           ///< Log file handle
    static QTextStream* s_logStream;   ///< Log file stream
    static Level s_minLevel;           ///< Minimum log level
    static bool s_enableConsole;       ///< Console logging flag
    static QMutex s_mutex;             ///< Thread safety mutex
    static QtMessageHandler s_oldHandler; ///< Original Qt message handler
};

#endif // LOGGER_H
