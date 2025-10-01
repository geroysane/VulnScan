#ifndef IAIPROVIDER_H
#define IAIPROVIDER_H

#include <QString>
#include <QList>
#include "scanner/Vulnerability.h"
#include "AIAnalysis.h"

/**
 * @interface IAIProvider
 * @brief Interface for AI analysis providers
 *
 * This interface defines the contract for AI-based vulnerability analysis.
 * Implementations can use different AI backends (OpenAI, Claude, local models, etc.)
 */
class IAIProvider {
public:
    virtual ~IAIProvider() = default;

    /**
     * @brief Analyzes a list of vulnerabilities using AI
     * @param vulns List of vulnerabilities to analyze
     * @return AIAnalysis object containing the analysis results
     */
    virtual AIAnalysis analyze(const QList<Vulnerability>& vulns) = 0;

    /**
     * @brief Gets the name of the AI provider
     * @return Provider name (e.g., "OpenAI GPT-4", "Claude", etc.)
     */
    virtual QString getName() const = 0;
};

#endif // IAIPROVIDER_H
