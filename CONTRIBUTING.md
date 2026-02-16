# Contributing to Legal Server

Thank you for considering contributing to Legal Server! This document provides guidelines for contributing to the project.

## 🚀 Development Process

### 1. Fork & Clone
```bash
git clone https://github.com/YOUR_USERNAME/legal-server.git
cd legal-server
```

### 2. Create Feature Branch
```bash
git checkout -b feature/your-feature-name
# or
git checkout -b fix/bug-description
```

### 3. Make Changes
- Write clean, well-documented code
- Follow C++20 best practices
- Add tests for new features
- Update documentation as needed

### 4. Test Your Changes
```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make -j$(nproc)

# Run tests
./legal-server-tests

# Test manually
./legal-server ../config/dev.json
```

### 5. Commit Changes
```bash
git add .
git commit -m "feat: Add amazing feature"
```

### 6. Push & Create Pull Request
```bash
git push origin feature/your-feature-name
```

Then open a Pull Request on GitHub.

## 📝 Coding Standards

### C++ Style Guide

**Naming Conventions:**
- **Classes**: `PascalCase` (e.g., `HttpServer`, `LegalService`)
- **Functions**: `camelCase` (e.g., `handleRequest`, `simplifyClause`)
- **Variables**: `snake_case` (e.g., `connection_pool`, `request_count`)
- **Constants**: `UPPER_SNAKE_CASE` (e.g., `MAX_CONNECTIONS`, `DEFAULT_PORT`)
- **Private members**: `snake_case_` with trailing underscore (e.g., `logger_`, `io_context_`)

**Code Style:**
```cpp
// Good: Clear, documented, follows conventions
class HttpServer {
public:
    HttpServer(const std::string& host, int port);
    void start();
    void stop();

private:
    void handleRequest();
    std::unique_ptr<boost::asio::io_context> io_context_;
    std::shared_ptr<Logger> logger_;
};

// Bad: Poor naming, no documentation
class server {
public:
    void DoStuff();
    int x;
};
```

### File Organization
- **Headers**: `include/[component]/ClassName.h`
- **Implementation**: `src/[component]/ClassName.cpp`
- **Tests**: `tests/[component]/ClassNameTest.cpp`

### Documentation
Use Doxygen-style comments for public APIs:
```cpp
/**
 * @brief Simplifies a legal clause using T5 model
 * @param clause The legal text to simplify
 * @return SimplificationResult containing simplified text and confidence score
 * @throws std::invalid_argument if clause is empty
 */
SimplificationResult simplifyClause(const std::string& clause);
```

## 🧪 Testing Guidelines

### Unit Tests
- Write tests for all new features
- Use Google Test framework
- Aim for >80% code coverage
- Test edge cases and error conditions

Example:
```cpp
TEST(LegalServiceTest, SimplifyValidClause) {
    auto service = createTestService();
    auto result = service->simplifyClause("Complex legal text");

    EXPECT_TRUE(result.success);
    EXPECT_GT(result.confidence, 0.0);
    EXPECT_FALSE(result.simplified_text.empty());
}

TEST(LegalServiceTest, RejectsEmptyClause) {
    auto service = createTestService();
    EXPECT_THROW(service->simplifyClause(""), std::invalid_argument);
}
```

### Integration Tests
- Test component interactions
- Use test database/cache instances
- Clean up resources after tests

## 📋 Commit Message Format

We follow [Conventional Commits](https://www.conventionalcommits.org/):

```
<type>: <description>

[optional body]

[optional footer]
```

### Types
- `feat`: New feature
- `fix`: Bug fix
- `docs`: Documentation only changes
- `style`: Code style changes (formatting, semicolons, etc.)
- `refactor`: Code refactoring (no functional changes)
- `perf`: Performance improvements
- `test`: Adding or updating tests
- `chore`: Build process, dependency updates, tooling

### Examples
```bash
# Good commit messages
git commit -m "feat: Add Redis caching for ML inference results"
git commit -m "fix: Handle database connection timeout gracefully"
git commit -m "docs: Update API documentation with new endpoints"
git commit -m "test: Add unit tests for ConnectionPool"

# Bad commit messages
git commit -m "fixed stuff"
git commit -m "WIP"
git commit -m "updates"
```

## 🔍 Pull Request Guidelines

### Before Submitting
- [ ] Code compiles without errors or warnings
- [ ] All tests pass
- [ ] Added tests for new features
- [ ] Updated documentation
- [ ] Followed coding standards
- [ ] Commit messages follow conventions

### PR Description Template
```markdown
## Description
Brief description of changes

## Type of Change
- [ ] Bug fix
- [ ] New feature
- [ ] Breaking change
- [ ] Documentation update

## Testing
Describe how you tested the changes

## Checklist
- [ ] Code follows project style guidelines
- [ ] Self-reviewed the code
- [ ] Added comments for complex logic
- [ ] Updated documentation
- [ ] No new warnings
- [ ] Added tests
- [ ] All tests pass
```

## 🐛 Reporting Bugs

### Bug Report Template
```markdown
**Describe the bug**
Clear description of the bug

**To Reproduce**
Steps to reproduce:
1. Start server with config X
2. Send request Y
3. Observe error Z

**Expected behavior**
What you expected to happen

**Environment**
- OS: Ubuntu 22.04
- Compiler: GCC 11.3
- CMake: 3.24
- Dependencies: Boost 1.81, PostgreSQL 15

**Logs**
Relevant log output
```

## 💡 Feature Requests

### Feature Request Template
```markdown
**Problem**
What problem does this feature solve?

**Proposed Solution**
How should it work?

**Alternatives**
Other solutions you considered

**Additional Context**
Any other relevant information
```

## 📚 Resources

- [C++20 Reference](https://en.cppreference.com/)
- [Boost.Asio Documentation](https://www.boost.org/doc/libs/release/doc/html/boost_asio.html)
- [PostgreSQL C Library](https://www.postgresql.org/docs/current/libpq.html)
- [Google Test Guide](https://google.github.io/googletest/)
- [CMake Documentation](https://cmake.org/documentation/)

## ❓ Questions?

If you have questions:
1. Check existing [Issues](https://github.com/YOUR_USERNAME/legal-server/issues)
2. Search [Discussions](https://github.com/YOUR_USERNAME/legal-server/discussions)
3. Open a new Issue with the `question` label

## 🙏 Thank You!

Your contributions help make Legal Server better for everyone. We appreciate your time and effort!

---

**Happy Coding!** 🚀
