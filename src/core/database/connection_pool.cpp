#include "core/database/connection_pool.h"
#include <iostream>
#include <thread>

ConnectionPool::ConnectionPool(const std::string& conn_str, int size, int max)
    : connection_string(conn_str), pool_size(size), max_connections(max) {}

ConnectionPool::~ConnectionPool() {
    shutdown();
}

void ConnectionPool::initialize() {
    std::lock_guard<std::mutex> lock(pool_mutex);
    
    for (int i = 0; i < pool_size; ++i) {
        auto conn = std::make_shared<DatabaseConnection>(connection_string);
        try {
            conn->connect();
            available_connections.push(conn);
        } catch (const std::exception& e) {
            std::cerr << "Failed to initialize connection " << i << ": " << e.what() << std::endl;
        }
    }
    
    std::cout << "✓ Connection pool initialized with " << pool_size << " connections" << std::endl;
}

std::shared_ptr<DatabaseConnection> ConnectionPool::getConnection() {
    std::lock_guard<std::mutex> lock(pool_mutex);
    
    if (available_connections.empty()) {
        if (used_connections.size() < max_connections) {
            auto conn = std::make_shared<DatabaseConnection>(connection_string);
            conn->connect();
            used_connections.push(conn);
            return conn;
        }
        throw std::runtime_error("No available database connections");
    }
    
    auto conn = available_connections.front();
    available_connections.pop();
    used_connections.push(conn);
    
    return conn;
}

void ConnectionPool::releaseConnection(std::shared_ptr<DatabaseConnection> conn) {
    std::lock_guard<std::mutex> lock(pool_mutex);
    available_connections.push(conn);
}

int ConnectionPool::getAvailableConnections() const {
    std::lock_guard<std::mutex> lock(pool_mutex);
    return available_connections.size();
}

int ConnectionPool::getUsedConnections() const {
    std::lock_guard<std::mutex> lock(pool_mutex);
    return used_connections.size();
}

void ConnectionPool::shutdown() {
    std::lock_guard<std::mutex> lock(pool_mutex);
    
    while (!available_connections.empty()) {
        available_connections.pop();
    }
    
    while (!used_connections.empty()) {
        used_connections.pop();
    }
    
    std::cout << "✓ Connection pool shutdown" << std::endl;
}
