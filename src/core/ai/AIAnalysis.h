#ifndef AIANALYSIS_H
#define AIANALYSIS_H

#include <QString>
#include <QMetaType>

/**
 * @struct AIAnalysis
 * @brief Contains results from AI-based vulnerability analysis
 *
 * This structure holds the output of AI analysis including
 * risk assessment, recommendations, and detailed insights.
 */
struct AIAnalysis {
    QString summary;           ///< Executive summary of findings
    QString riskLevel;         ///< Overall risk level (Critical, High, Medium, Low)
    QString recommendations;   ///< AI-generated remediation recommendations
    QString detailedAnalysis;  ///< Detailed technical analysis
    float confidenceScore;     ///< AI confidence score (0.0 - 1.0)

    AIAnalysis() : confidenceScore(0.0f) {}
};

Q_DECLARE_METATYPE(AIAnalysis)

#endif // AIANALYSIS_H
