# FASE 7: DevOps & Distribution 📦

**Obiettivo**: Deployment e distribuzione cross-platform

## 🎯 Obiettivi

### CI/CD Pipeline
- GitHub Actions workflows
- Automated builds (Linux/Windows/macOS)
- Automated testing
- Code quality checks (clang-format, CodeQL)

### Build Automation
- qmake build scripts
- Qt deployment tools
- Dependency bundling

### Packaging
- **Linux**: AppImage, .deb, .rpm
- **Windows**: NSIS installer, portable exe
- **macOS**: DMG, .app bundle

### Release Automation
- Automated releases on tags
- Changelog generation
- Binary uploads
- SHA256 checksums

### Translation Pipeline
- lupdate/lrelease automation
- Translation file management

### Documentation Generation
- Doxygen in CI
- API docs publishing
- Documentation website

## 📋 GitHub Actions Workflows

### .github/workflows/build.yml
- Matrix build (Ubuntu, Windows, macOS)
- Install Qt via aqtinstall
- Run qmake + make
- Run tests
- Upload artifacts

### .github/workflows/release.yml
- Trigger on v*.*.* tags
- Build all platforms
- Generate installers
- Create GitHub release
- Upload binaries

### .github/workflows/codeql.yml
- Security scanning
- CodeQL analysis

## ✅ Deliverable

- [x] GitHub Actions CI/CD
- [x] Cross-platform builds
- [x] Packaging scripts
- [x] Release automation
- [x] Translation pipeline
- [x] Doxygen CI integration
- [x] Documentation website

## 🔄 Next: FASE 8 - Documentation & Community