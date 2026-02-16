#!/bin/bash
set -e

echo "Installing Legal Server Dependencies..."

# Update package list
sudo apt-get update

# Install build tools
sudo apt-get install -y \
    build-essential \
    cmake \
    git \
    pkg-config \
    wget \
    curl

# Install C++ libraries
sudo apt-get install -y \
    libboost-all-dev \
    nlohmann-json3-dev \
    libspdlog-dev \
    libpq-dev \
    libssl-dev \
    libcurl4-openssl-dev

# Install database/cache clients
sudo apt-get install -y \
    postgresql-client \
    redis-tools

# Verify installations
echo "Verifying installations..."
g++ --version
cmake --version
echo "✅ All dependencies installed successfully!"
