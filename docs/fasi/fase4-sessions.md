# FASE 4: AI Integration - Development Sessions

This document tracks the detailed progress of FASE 4 implementation, broken down into development sessions.

**Overall FASE 4 Progress**: 0/5 sessions completed (0%)

---

## Proposed Session Breakdown

### Session 1: AI Provider Foundation (~2h)
**Goal**: Implement base AI provider architecture and GenericProvider

**Tasks**:
- [ ] Review existing IAIProvider interface
- [ ] Implement GenericProvider.h/cpp (custom endpoints)
- [ ] Add support for custom headers
- [ ] Add API format selector (OpenAI-compatible vs Anthropic-compatible)
- [ ] Implement connection testing
- [ ] Create tests/TestGenericProvider.h/cpp
- [ ] Test with mock endpoints

**Deliverables**:
- GenericProvider fully functional
- Support for OpenRouter, Groq, Ollama presets
- Unit tests for provider functionality

**Files to Create**:
- src/core/ai/GenericProvider.h
- src/core/ai/GenericProvider.cpp
- tests/TestGenericProvider.h
- tests/TestGenericProvider.cpp

---

### Session 2: Official AI Providers (~2h)
**Goal**: Implement Claude, OpenAI, and Gemini providers

**Tasks**:
- [ ] Implement ClaudeProvider.h/cpp (Anthropic API)
- [ ] Implement OpenAIProvider.h/cpp (OpenAI API)
- [ ] Implement GeminiProvider.h/cpp (Google API)
- [ ] Add API key management (environment variables)
- [ ] Implement request/response formatting for each API
- [ ] Create tests/TestAIProviders.h/cpp
- [ ] Test with real/mock API calls

**Deliverables**:
- Three official providers working
- API key security (env variables)
- Unit tests for each provider

**Files to Create**:
- src/core/ai/ClaudeProvider.h
- src/core/ai/ClaudeProvider.cpp
- src/core/ai/OpenAIProvider.h
- src/core/ai/OpenAIProvider.cpp
- src/core/ai/GeminiProvider.h
- src/core/ai/GeminiProvider.cpp
- tests/TestAIProviders.h
- tests/TestAIProviders.cpp

---

### Session 3: Async Workers & Rate Limiting (~1.5h)
**Goal**: Implement async AI calls with thread workers and rate limiting

**Tasks**:
- [ ] Implement AIWorker.h/cpp (QThread-based worker)
- [ ] Add rate limiting mechanism (token bucket or similar)
- [ ] Implement timeout management
- [ ] Add request queuing
- [ ] Create tests/TestAIWorker.h/cpp
- [ ] Test concurrent requests and rate limits

**Deliverables**:
- AIWorker for non-blocking API calls
- Thread-safe rate limiting
- Timeout handling
- Unit tests for async operations

**Files to Create**:
- src/core/ai/AIWorker.h
- src/core/ai/AIWorker.cpp
- src/core/ai/RateLimiter.h (optional - if separate class)
- src/core/ai/RateLimiter.cpp
- tests/TestAIWorker.h
- tests/TestAIWorker.cpp

---

### Session 4: AI Analyzer & Fallback (~1.5h)
**Goal**: Implement orchestration layer with multiple providers and fallback

**Tasks**:
- [ ] Implement AIAnalyzer.h/cpp (orchestrator)
- [ ] Add concurrent provider calls
- [ ] Implement automatic fallback mechanism
- [ ] Add provider priority/preference system
- [ ] Create tests/TestAIAnalyzer.h/cpp
- [ ] Test fallback scenarios

**Deliverables**:
- AIAnalyzer orchestrating multiple providers
- Automatic fallback working
- Provider selection logic
- Unit tests for orchestration

**Files to Create**:
- src/core/ai/AIAnalyzer.h
- src/core/ai/AIAnalyzer.cpp
- tests/TestAIAnalyzer.h
- tests/TestAIAnalyzer.cpp

---

### Session 5: Prompt Builder & Integration (~1.5h)
**Goal**: Implement prompt generation and integrate with vulnerability scanner

**Tasks**:
- [ ] Implement PromptBuilder.h/cpp
- [ ] Design prompt templates for vulnerability analysis
- [ ] Add context injection (scan results, vulnerabilities)
- [ ] Integrate AIAnalyzer with VulnerabilityScanner
- [ ] Update Configuration for AI settings
- [ ] Create integration tests
- [ ] Test end-to-end AI analysis flow

**Deliverables**:
- PromptBuilder generating effective prompts
- AI analysis integrated into scan workflow
- Configuration support for AI settings
- Integration tests for complete flow

**Files to Create**:
- src/core/ai/PromptBuilder.h
- src/core/ai/PromptBuilder.cpp
- tests/TestPromptBuilder.h
- tests/TestPromptBuilder.cpp
- Update Configuration.h/cpp for AI settings
- Integration tests in IntegrationTestsAI.h/cpp

---

## Session Tracking

| Session | Status | Started | Completed | Duration | Notes |
|---------|--------|---------|-----------|----------|-------|
| 1 - AI Provider Foundation | ⏳ Pending | - | - | ~2 h | GenericProvider + connection testing |
| 2 - Official AI Providers | ⏳ Pending | - | - | ~2 h | Claude, OpenAI, Gemini |
| 3 - Async Workers & Rate Limiting | ⏳ Pending | - | - | ~1.5 h | AIWorker + thread safety |
| 4 - AI Analyzer & Fallback | ⏳ Pending | - | - | ~1.5 h | Orchestration + fallback |
| 5 - Prompt Builder & Integration | ⏳ Pending | - | - | ~1.5 h | Prompts + end-to-end |

**Total Estimated Time**: 8.5 hours
**Actual Time**: TBD
**Progress**: 0/5 sessions completed (0%)

---

## Overall FASE 4 Checklist

Before marking FASE 4 as complete:

- [ ] All 5 sessions completed
- [ ] GenericProvider working with custom endpoints
- [ ] ClaudeProvider, OpenAIProvider, GeminiProvider implemented
- [ ] AIWorker async operations functional
- [ ] Rate limiting working and thread-safe
- [ ] AIAnalyzer orchestration with fallback
- [ ] PromptBuilder generating effective prompts
- [ ] AI analysis integrated into scan workflow
- [ ] Configuration support for AI settings
- [ ] All unit tests passing
- [ ] Integration tests created and passing
- [ ] Documentation updated
- [ ] Code committed to Git
- [ ] Ready for FASE 5 (GUI Application)

---

## Implementation Notes

### Security Considerations
- **API Keys**: Must use environment variables (QProcessEnvironment::systemEnvironment())
- **Never commit API keys** to repository
- **HTTPS only** for all API endpoints
- **Validate responses** before processing

### API Rate Limits (Typical)
- **OpenAI**: 60 requests/minute (tier 1)
- **Anthropic (Claude)**: 50 requests/minute (free tier)
- **Google (Gemini)**: 60 requests/minute (free tier)
- **OpenRouter**: Varies by model and account

### Prompt Engineering Best Practices
- **Be specific**: Clear instructions for vulnerability analysis
- **Provide context**: Include service info, version, scan results
- **Structure output**: Request JSON or structured format
- **Set constraints**: Token limits, response format
- **Examples**: Few-shot examples for better results

### Testing Strategy
- **Mock API responses** for unit tests (no real API calls)
- **Use test API keys** for integration tests (optional)
- **Test error handling**: network failures, timeouts, invalid responses
- **Test rate limiting**: concurrent requests should queue properly
- **Test fallback**: verify secondary provider used when primary fails

---

**Last Updated**: 2025-10-03
**Current Status**: Planning complete - Ready to start Session 1
