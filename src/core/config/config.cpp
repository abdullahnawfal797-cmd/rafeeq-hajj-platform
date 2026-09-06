#include "core/config/config.h"
#include <fstream>
#include <iostream>

Config::Config() {
    // Default configuration
    config_data = json::object();
}

void Config::loadFromFile(const std::string& file_path) {
    std::ifstream config_stream(file_path);
    if (!config_stream.is_open()) {
        throw std::runtime_error("Cannot open config file: " + file_path);
    }
    
    try {
        config_stream >> config_data;
        config_file = file_path;
    } catch (const json::exception& e) {
        throw std::runtime_error("Invalid JSON in config file: " + std::string(e.what()));
    }
}

int Config::getServerPort() const {
    return config_data.value("server_port", 8080);
}

std::string Config::getDatabaseUrl() const {
    return config_data.value("database_url", "postgresql://localhost:5432/rafeeq");
}

std::string Config::getRedisUrl() const {
    return config_data.value("redis_url", "redis://localhost:6379");
}

std::string Config::getGoogleMapsApiKey() const {
    return config_data.value("google_maps_api_key", "");
}

bool Config::isDevelopment() const {
    return config_data.value("environment", "development") == "development";
}

void Config::setServerPort(int port) {
    config_data["server_port"] = port;
}

void Config::setDatabaseUrl(const std::string& url) {
    config_data["database_url"] = url;
}

void Config::setRedisUrl(const std::string& url) {
    config_data["redis_url"] = url;
}
