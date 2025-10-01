#include "Logger.h"
#include <QDateTime>
#include <QMutexLocker>
#include <iostream>

// Static member initialization
QFile* Logger::s_logFile = nullptr;
QTextStream* Logger::s_logStream = nullptr;
Logger::Level Logger::s_minLevel = Level::Info;
bool Logger::s_enableConsole = true;
QMutex Logger::s_mutex;
QtMessageHandler Logger::s_oldHandler = nullptr;

void Logger::initialize(const QString& logFilePath, bool enableConsole) {
    QMutexLocker locker(&s_mutex);

    s_enableConsole = enableConsole;

    // Create log file
    s_logFile = new QFile(logFilePath);
    if (s_logFile->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        s_logStream = new QTextStream(s_logFile);
        s_logStream->setEncoding(QStringConverter::Utf8);
    } else {
        std::cerr << "Failed to open log file: " << logFilePath.toStdString() << std::endl;
        delete s_logFile;
        s_logFile = nullptr;
    }

    // Install custom message handler for Qt messages
    s_oldHandler = qInstallMessageHandler(messageHandler);

    log(Level::Info, "Logger initialized - " + logFilePath);
}

void Logger::log(Level level, const QString& message) {
    QMutexLocker locker(&s_mutex);

    // Check if level is sufficient
    if (level < s_minLevel) {
        return;
    }

    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz");
    QString levelStr = levelToString(level);
    QString logLine = QString("[%1] [%2] %3").arg(timestamp, levelStr, message);

    // Write to file
    if (s_logStream) {
        (*s_logStream) << logLine << Qt::endl;
        s_logStream->flush();
    }

    // Write to console
    if (s_enableConsole) {
        if (level >= Level::Error) {
            std::cerr << logLine.toStdString() << std::endl;
        } else {
            std::cout << logLine.toStdString() << std::endl;
        }
    }
}

void Logger::setLogLevel(Level level) {
    QMutexLocker locker(&s_mutex);
    s_minLevel = level;
}

void Logger::shutdown() {
    QMutexLocker locker(&s_mutex);

    log(Level::Info, "Logger shutting down");

    if (s_logStream) {
        delete s_logStream;
        s_logStream = nullptr;
    }

    if (s_logFile) {
        s_logFile->close();
        delete s_logFile;
        s_logFile = nullptr;
    }

    // Restore old message handler
    if (s_oldHandler) {
        qInstallMessageHandler(s_oldHandler);
        s_oldHandler = nullptr;
    }
}

void Logger::debug(const QString& message) {
    log(Level::Debug, message);
}

void Logger::info(const QString& message) {
    log(Level::Info, message);
}

void Logger::warning(const QString& message) {
    log(Level::Warning, message);
}

void Logger::error(const QString& message) {
    log(Level::Error, message);
}

void Logger::critical(const QString& message) {
    log(Level::Critical, message);
}

void Logger::messageHandler(QtMsgType type,
                           const QMessageLogContext& context,
                           const QString& msg) {
    Level level = qtMsgTypeToLevel(type);
    QString contextInfo;

    if (context.file) {
        contextInfo = QString(" (%1:%2, %3)")
                         .arg(context.file)
                         .arg(context.line)
                         .arg(context.function ? context.function : "");
    }

    log(level, msg + contextInfo);

    // Call old handler if it exists
    if (s_oldHandler) {
        s_oldHandler(type, context, msg);
    }
}

QString Logger::levelToString(Level level) {
    switch (level) {
        case Level::Debug:    return "DEBUG";
        case Level::Info:     return "INFO ";
        case Level::Warning:  return "WARN ";
        case Level::Error:    return "ERROR";
        case Level::Critical: return "CRIT ";
        default:              return "UNKNOWN";
    }
}

Logger::Level Logger::qtMsgTypeToLevel(QtMsgType type) {
    switch (type) {
        case QtDebugMsg:    return Level::Debug;
        case QtInfoMsg:     return Level::Info;
        case QtWarningMsg:  return Level::Warning;
        case QtCriticalMsg: return Level::Error;
        case QtFatalMsg:    return Level::Critical;
        default:            return Level::Info;
    }
}
