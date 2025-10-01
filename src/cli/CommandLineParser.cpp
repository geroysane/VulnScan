#include "CommandLineParser.h"
#include <QCoreApplication>
#include <QDebug>

CommandLineParser::CommandLineParser()
    : m_portRange("1-1000")
{
    m_parser.setApplicationDescription("VulnScan - Network Vulnerability Scanner (CLI)");
    m_parser.addHelpOption();
    m_parser.addVersionOption();

    // Add positional argument for target
    m_parser.addPositionalArgument("target", "Target host or IP address to scan");

    // Add optional arguments
    QCommandLineOption portOption(
        QStringList() << "p" << "ports",
        "Port range to scan (default: 1-1000)",
        "range",
        "1-1000"
    );
    m_parser.addOption(portOption);
}

bool CommandLineParser::parse(QCoreApplication& app) {
    m_parser.process(app);

    // Get positional arguments
    const QStringList args = m_parser.positionalArguments();
    if (args.isEmpty()) {
        qWarning() << "Error: No target specified";
        m_parser.showHelp(1);
        return false;
    }

    m_target = args.first();
    m_portRange = m_parser.value("ports");

    return true;
}

QString CommandLineParser::getTarget() const {
    return m_target;
}

QString CommandLineParser::getPortRange() const {
    return m_portRange;
}
