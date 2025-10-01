#ifndef COMMANDLINEPARSER_H
#define COMMANDLINEPARSER_H

#include <QCommandLineParser>
#include <QString>

/**
 * @class CommandLineParser
 * @brief Handles command line argument parsing for VulnScan CLI
 *
 * This is a placeholder for command line parsing.
 * Full implementation will be done in FASE 3.
 */
class CommandLineParser {
public:
    CommandLineParser();

    /**
     * @brief Parse command line arguments
     * @param app QCoreApplication instance
     * @return true if parsing succeeded, false otherwise
     */
    bool parse(QCoreApplication& app);

    /**
     * @brief Get the target host/IP to scan
     * @return Target host
     */
    QString getTarget() const;

    /**
     * @brief Get the port range to scan
     * @return Port range string (e.g., "1-1000")
     */
    QString getPortRange() const;

private:
    QCommandLineParser m_parser;
    QString m_target;
    QString m_portRange;
};

#endif // COMMANDLINEPARSER_H
