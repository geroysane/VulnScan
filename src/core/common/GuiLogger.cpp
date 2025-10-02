#include "GuiLogger.h"
#include <QMutexLocker>

GuiLogger* GuiLogger::s_instance = nullptr;

GuiLogger::GuiLogger(QObject* parent)
    : QObject(parent)
    , m_minLevel(Debug)
    , m_fileLoggingEnabled(false)
    , m_logFile(nullptr)
    , m_logStream(nullptr)
{
}

GuiLogger::~GuiLogger()
{
    if (m_logStream) {
        delete m_logStream;
        m_logStream = nullptr;
    }

    if (m_logFile) {
        if (m_logFile->isOpen()) {
            m_logFile->close();
        }
        delete m_logFile;
        m_logFile = nullptr;
    }
}

GuiLogger* GuiLogger::instance()
{
    if (!s_instance) {
        s_instance = new GuiLogger();
    }
    return s_instance;
}

bool GuiLogger::setFileLogging(bool enabled, const QString& filePath)
{
    QMutexLocker locker(&m_mutex);

    // Clean up existing file logging
    if (m_logStream) {
        delete m_logStream;
        m_logStream = nullptr;
    }

    if (m_logFile) {
        if (m_logFile->isOpen()) {
            m_logFile->close();
        }
        delete m_logFile;
        m_logFile = nullptr;
    }

    m_fileLoggingEnabled = false;

    if (enabled && !filePath.isEmpty()) {
        m_logFile = new QFile(filePath);
        if (m_logFile->open(QIODevice::Append | QIODevice::Text)) {
            m_logStream = new QTextStream(m_logFile);
            m_logFilePath = filePath;
            m_fileLoggingEnabled = true;
            return true;
        } else {
            delete m_logFile;
            m_logFile = nullptr;
            return false;
        }
    }

    return true;
}

void GuiLogger::setLogLevel(LogLevel level)
{
    QMutexLocker locker(&m_mutex);
    m_minLevel = level;
}

void GuiLogger::debug(const QString& message, const QString& context)
{
    log(Debug, message, context);
}

void GuiLogger::info(const QString& message, const QString& context)
{
    log(Info, message, context);
}

void GuiLogger::warning(const QString& message, const QString& context)
{
    log(Warning, message, context);
}

void GuiLogger::error(const QString& message, const QString& context)
{
    log(Error, message, context);
}

void GuiLogger::critical(const QString& message, const QString& context)
{
    log(Critical, message, context);
}

void GuiLogger::log(LogLevel level, const QString& message, const QString& context)
{
    // Check log level threshold
    if (level < m_minLevel) {
        return;
    }

    QDateTime timestamp = QDateTime::currentDateTime();

    // Emit signal for GUI components
    emit logMessage(level, timestamp, context, message);

    // Write to file if enabled
    if (m_fileLoggingEnabled) {
        writeToFile(level, timestamp, context, message);
    }
}

QString GuiLogger::levelToString(LogLevel level) const
{
    switch (level) {
        case Debug:    return "DEBUG";
        case Info:     return "INFO";
        case Warning:  return "WARNING";
        case Error:    return "ERROR";
        case Critical: return "CRITICAL";
        default:       return "UNKNOWN";
    }
}

void GuiLogger::writeToFile(LogLevel level, const QDateTime& timestamp,
                            const QString& context, const QString& message)
{
    QMutexLocker locker(&m_mutex);

    if (!m_fileLoggingEnabled || !m_logStream) {
        return;
    }

    QString logLine = QString("[%1] [%2]")
                        .arg(timestamp.toString("yyyy-MM-dd HH:mm:ss.zzz"))
                        .arg(levelToString(level));

    if (!context.isEmpty()) {
        logLine += QString(" [%1]").arg(context);
    }

    logLine += QString(" %1").arg(message);

    *m_logStream << logLine << "\n";
    m_logStream->flush();
}
