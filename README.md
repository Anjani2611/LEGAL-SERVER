# Legal Server

High-performance C++ backend for legal document simplification and classification using machine learning.

## 🎯 Overview

Legal Server is a production-grade C++ backend system that processes legal documents, simplifies complex clauses, and classifies legal text using state-of-the-art NLP models (T5 for simplification, Legal-BERT for classification).

### Key Features

- **Async HTTP Server**: Boost.Asio-based server handling 5000+ concurrent connections
- **PostgreSQL Integration**: Connection pooling with RAII for safe database operations
- **Redis Caching**: Sub-millisecond response times with intelligent caching
- **ML Integration**: T5 & Legal-BERT models for text simplification and classification
- **Clean Architecture**: Layered design with SOLID principles
- **Production-Ready**: Docker/Kubernetes deployment, monitoring, security

## 🏗️ Architecture

```
┌─────────────────────────────┐
│  HTTP Server (Boost.Asio)   │  Port 8080, async I/O
├─────────────────────────────┤
│  Request Handler            │  Route matching, validation
├─────────────────────────────┤
│  Legal Service              │  Business logic orchestration
├─────────────────────────────┤
│  Document Repository        │  PostgreSQL data access
├─────────────────────────────┤
│  Redis Cache                │  Fast result memoization
├─────────────────────────────┤
│  ML Inference Service       │  Model integration
└─────────────────────────────┘
```

## 🛠️ Technology Stack

- **Language**: C++20
- **Build System**: CMake 3.20+
- **HTTP Server**: Boost.Asio 1.81+
- **Database**: PostgreSQL 15+
- **Cache**: Redis 7+
- **Logging**: spdlog
- **JSON**: nlohmann/json
- **Testing**: Google Test
- **ML Models**: T5-base, Legal-BERT
- **Deployment**: Docker, Kubernetes

## 📋 Prerequisites

### System Requirements
- Ubuntu 22.04+ / Debian 11+ / Windows with WSL2
- GCC 11+ or Clang 14+
- CMake 3.20+
- 4GB RAM minimum (8GB recommended)
- PostgreSQL 15+
- Redis 7+

### Dependencies
```bash
# Ubuntu/Debian/WSL2
sudo apt-get update && sudo apt-get install -y \
    build-essential cmake git \
    libboost-all-dev \
    nlohmann-json3-dev \
    libspdlog-dev \
    libpq-dev \
    libssl-dev \
    postgresql-client \
    redis-tools
```

## 🚀 Quick Start

### 1. Clone Repository
```bash
git clone https://github.com/YOUR_USERNAME/legal-server.git
cd legal-server
```

### 2. Build
```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc)
```

### 3. Configure
```bash
cp config/dev.json.template config/dev.json
# Edit config/dev.json with your database credentials
```

### 4. Run
```bash
./legal-server ../config/dev.json
```

### 5. Test
```bash
curl http://127.0.0.1:8080/health
```

Expected response:
```json
{
  "status": "ok",
  "service": "legal-server",
  "timestamp": 1708095360
}
```

## 📖 API Documentation

### Health Check
```http
GET /health
```

### Simplify Clause
```http
POST /api/v1/simplify
Content-Type: application/json

{
  "clause": "Notwithstanding any provisions herein to the contrary..."
}
```

Response:
```json
{
  "status": "success",
  "data": {
    "simplified": "Despite any terms stated here...",
    "confidence": 0.92,
    "inference_time_ms": 150
  }
}
```

### Classify Clause
```http
POST /api/v1/classify
Content-Type: application/json

{
  "clause": "Payment shall be due within 30 days..."
}
```

Response:
```json
{
  "status": "success",
  "data": {
    "classification": "payment_terms",
    "confidence": 0.87
  }
}
```

### Upload Document
```http
POST /api/v1/upload
Content-Type: application/json

{
  "filename": "contract.txt",
  "content": "Full document text..."
}
```

Full API documentation: [docs/API.md](docs/API.md)

## 🧪 Testing

### Run Unit Tests
```bash
cd build
./legal-server-tests
```

### Load Testing
```bash
wrk -t4 -c100 -d30s http://127.0.0.1:8080/health
```

## 🐳 Docker Deployment

### Build Image
```bash
docker build -t legal-server:latest .
```

### Run Container
```bash
docker run -p 8080:8080 \
  -e DB_CONNECTION_STRING="postgresql://..." \
  -e REDIS_URI="localhost:6379" \
  legal-server:latest
```

### Docker Compose
```bash
docker-compose up -d
```

## ☸️ Kubernetes Deployment

```bash
kubectl apply -f kubernetes/deployment.yaml
kubectl apply -f kubernetes/service.yaml
```

## 📊 Project Structure

```
legal-server/
├── include/               # Header files
│   ├── server/           # HTTP server components
│   ├── services/         # Business logic
│   ├── repository/       # Data access layer
│   ├── cache/            # Redis cache wrapper
│   └── utils/            # Logging, config, helpers
├── src/                  # Implementation files
│   ├── server/
│   ├── services/
│   ├── repository/
│   ├── cache/
│   └── utils/
├── tests/                # Unit and integration tests
├── config/               # Configuration files
│   ├── dev.json.template
│   └── prod.json.template
├── docker/               # Docker files
│   ├── Dockerfile
│   └── docker-compose.yml
├── kubernetes/           # Kubernetes manifests
├── docs/                 # Documentation
├── scripts/              # Utility scripts
├── models/               # ML models
└── CMakeLists.txt        # Build configuration
```

## 🗓️ Development Roadmap

- [x] **Week 1**: Project structure and build system
- [ ] **Week 2**: HTTP Server foundation (Boost.Asio)
- [ ] **Week 3**: Database layer (PostgreSQL, Redis)
- [ ] **Week 4**: API routes & business logic
- [ ] **Week 5**: ML model integration
- [ ] **Week 6**: Production deployment

## 🤝 Contributing

Contributions are welcome! Please read [CONTRIBUTING.md](CONTRIBUTING.md) for details.

### Development Setup
1. Fork the repository
2. Create feature branch: `git checkout -b feature/amazing-feature`
3. Commit changes: `git commit -m 'feat: Add amazing feature'`
4. Push to branch: `git push origin feature/amazing-feature`
5. Open Pull Request

## 📝 License

This project is licensed under the MIT License - see [LICENSE](LICENSE) file for details.

## 👤 Author

**Anjani**
- GitHub: [@anjani2611](https://github.com/anjani2611)
- Project Link: [https://github.com/anjani2611/legal-server](https://github.com/anjani2611/legal-server)

## 🙏 Acknowledgments

- Boost.Asio for async networking
- PostgreSQL team for excellent documentation
- spdlog for fast logging
- Legal-BERT authors for pre-trained models
- T5 team at Google Research

## 📧 Contact

For questions or support, please open an issue on GitHub.

---

**Status**: 🚧 Active Development | **Version**: 0.1.0 | **Last Updated**: February 2026
