# FASE 4: AI Integration 🤖

**Obiettivo**: Analisi intelligente con AI e supporto provider multipli

## 🎯 Obiettivi

- AI provider interfaces (Claude, OpenAI, Gemini)
- GenericProvider per custom endpoints
- Async AI calls con thread workers
- Multiple provider support con fallback
- Rate limiting thread-safe
- Custom headers support
- Provider presets (OpenRouter, Groq, Ollama)

## 📦 Componenti

### IAIProvider Interface
```cpp
class IAIProvider {
public:
    virtual AIAnalysis analyze(const QList<Vulnerability>& vulns) = 0;
    virtual QString getName() const = 0;
};
```

### ClaudeProvider, OpenAIProvider, GeminiProvider
Provider specifici per API ufficiali

### GenericProvider
**Features**:
- Custom endpoint URL
- Custom API key
- Custom model identifier
- Custom headers (per OpenRouter, ecc.)
- API format selector (OpenAI-compatible, Anthropic-compatible)
- Connection testing

### AIWorker
Thread worker per chiamate API asincrone

### AIAnalyzer
Orchestrator per:
- Concurrent AI calls
- Automatic fallback
- Timeout management
- Rate limiting

### PromptBuilder
Context injection e prompt generation

## ⚙️ Configuration

```json
{
  "ai": {
    "default_provider": "openrouter",
    "providers": {
      "openrouter": {
        "type": "generic",
        "endpoint": "https://openrouter.ai/api/v1/chat/completions",
        "headers": {
          "HTTP-Referer": "https://github.com/vulnscan",
          "X-Title": "VulnScan"
        }
      }
    }
  }
}
```

## ✅ Deliverable

- [x] AI provider interfaces
- [x] GenericProvider implementation
- [x] AIWorker per async calls
- [x] Concurrent provider calls
- [x] Fallback mechanism
- [x] Custom headers support
- [x] Provider presets
- [x] Rate limiting
- [x] Connection testing
- [x] Unit tests

## 🔄 Next: FASE 5 - GUI Application