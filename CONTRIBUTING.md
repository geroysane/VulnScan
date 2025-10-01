# Contributing to VulnScan

Thank you for your interest in contributing to VulnScan! This document provides guidelines and instructions for contributing to the project.

## 🛡️ Code of Conduct

### Our Pledge

We are committed to making participation in this project a harassment-free experience for everyone, regardless of age, body size, disability, ethnicity, sex characteristics, gender identity and expression, level of experience, education, socio-economic status, nationality, personal appearance, race, religion, or sexual identity and orientation.

### Our Standards

**Positive behavior includes:**
- Using welcoming and inclusive language
- Being respectful of differing viewpoints and experiences
- Gracefully accepting constructive criticism
- Focusing on what is best for the community
- Showing empathy towards other community members

**Unacceptable behavior includes:**
- The use of sexualized language or imagery
- Trolling, insulting/derogatory comments, and personal or political attacks
- Public or private harassment
- Publishing others' private information without explicit permission
- Other conduct which could reasonably be considered inappropriate in a professional setting

### Enforcement

Project maintainers have the right and responsibility to remove, edit, or reject comments, commits, code, wiki edits, issues, and other contributions that are not aligned with this Code of Conduct.

## 🚀 How to Contribute

### Reporting Bugs

Before creating bug reports, please check existing issues to avoid duplicates. When creating a bug report, include:

- **Clear title and description**
- **Steps to reproduce** the problem
- **Expected behavior** vs actual behavior
- **Environment details** (OS, Qt version, compiler, etc.)
- **Screenshots or logs** if applicable
- **Minimal code example** that demonstrates the issue

### Suggesting Enhancements

Enhancement suggestions are welcome! Please provide:

- **Clear and descriptive title**
- **Detailed description** of the proposed functionality
- **Use case examples** demonstrating why this enhancement would be useful
- **Possible implementation approach** (if you have ideas)

### Pull Requests

1. **Fork the repository** and create your branch from `main`
2. **Follow the coding standards** outlined below
3. **Add tests** for any new functionality
4. **Ensure all tests pass** before submitting
5. **Update documentation** as needed
6. **Write clear commit messages** following our commit conventions
7. **Submit the pull request** with a comprehensive description

## 🏗️ Development Workflow

### Setting Up Your Development Environment

```bash
# Fork and clone the repository
git clone https://github.com/YOUR_USERNAME/VulnScan.git
cd VulnScan

# Create a feature branch
git checkout -b feature/your-feature-name

# Build the project
mkdir build && cd build
cmake -DCMAKE_PREFIX_PATH="/path/to/qt" ..
cmake --build . -j4

# Run tests
ctest --output-on-failure
```

### Branch Naming Convention

- `feature/description` - New features
- `fix/description` - Bug fixes
- `refactor/description` - Code refactoring
- `docs/description` - Documentation updates
- `test/description` - Test additions or modifications

### Commit Message Format

```
<type>(<scope>): <subject>

<body>

<footer>
```

**Types:**
- `feat`: New feature
- `fix`: Bug fix
- `refactor`: Code refactoring
- `test`: Test additions/modifications
- `docs`: Documentation changes
- `style`: Code style/formatting changes
- `perf`: Performance improvements
- `build`: Build system changes

**Example:**
```
feat(scanner): add TCP SYN scan support

Implement TCP SYN scanning for faster port detection.
Includes stealth mode and rate limiting options.

Closes #123
```

## 📝 Coding Standards

### C++ Style Guide

- **Language**: C++17
- **Indentation**: 4 spaces (no tabs)
- **Line length**: Maximum 100 characters
- **Naming conventions**:
  - Classes: PascalCase (e.g., NetworkScanner)
  - Functions/methods: camelCase (e.g., scanPort())
  - Variables: camelCase (e.g., portNumber)
  - Constants: UPPER_SNAKE_CASE (e.g., MAX_THREADS)
  - Private members: m_ prefix (e.g., m_database)
  - Interfaces: I prefix (e.g., IScanner)

### Code Organization

- **Single Responsibility Principle**: Each class/function should have one clear purpose
- **Header guards**: Use #pragma once
- **Include order**:
  1. Corresponding header (for .cpp files)
  2. Qt headers
  3. Standard library headers
  4. Third-party library headers
  5. Project headers

### Documentation

- **Doxygen comments** for all public APIs

### Qt Guidelines

- Use **Qt containers** (QVector, QMap, etc.) for Qt-interfacing code
- Use **Qt signals/slots** for event handling
- Prefer **Qt RAII classes** (QScopedPointer, QSharedPointer)
- Make strings **translatable** with tr() for UI text

### Testing

- Write **unit tests** for all new functionality
- Use **Qt Test framework**
- Aim for **high test coverage** (>80%)
- Tests should be **fast and independent**

## 🔒 Security Guidelines

### Responsible Disclosure

If you discover a security vulnerability:

1. **DO NOT** create a public GitHub issue
2. **Email the maintainers** privately
3. **Provide detailed information** about the vulnerability
4. **Allow reasonable time** for a fix before public disclosure

### Security Best Practices

- **No hardcoded credentials** or API keys
- **Validate all inputs** from users and external sources
- **Use secure defaults** for all configuration options
- **Handle sensitive data** securely (no logging of passwords, tokens, etc.)
- **Follow principle of least privilege**

## 📚 Project Phases

VulnScan is developed in 9 phases. Before contributing, familiarize yourself with:

- **Current phase**: See [project-state.md](project-state.md)
- **Phase details**: See [docs/fasi/README.md](docs/fasi/README.md)
- **Technical requirements**: See [docs/requirements.md](docs/requirements.md)

### Contributing to Current Phase

Focus contributions on the **current active phase** to maintain project coherence. If you want to work on future phases, please discuss with maintainers first.

## 🧪 Testing Your Changes

Before submitting a pull request:

```bash
# Build the project
cd build
cmake --build . -j4

# Run all tests
ctest --output-on-failure

# Run specific test
./tests/vulnscan_tests
```

## 📋 Pull Request Checklist

Before submitting your PR, ensure:

- [ ] Code follows the project coding standards
- [ ] All tests pass locally
- [ ] New tests added for new functionality
- [ ] Documentation updated (code comments, README, etc.)
- [ ] Commit messages follow the convention
- [ ] No merge conflicts with main branch
- [ ] Code compiles without warnings
- [ ] Changes are focused (one feature/fix per PR)

## 🤔 Questions?

If you have questions about contributing:

- **Check existing documentation** in docs/ folder
- **Search existing issues** for similar questions
- **Open a discussion** on GitHub Discussions
- **Contact maintainers** via GitHub issues

## 📜 License

By contributing to VulnScan, you agree that your contributions will be licensed under the MIT License.

## 🙏 Recognition

Contributors will be:
- Listed in the project contributors page
- Mentioned in release notes for significant contributions
- Credited in the AUTHORS file

---

Thank you for contributing to VulnScan and helping make network security tools more accessible!
