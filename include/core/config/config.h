#ifndef RAFEEQ_CONFIG_H
#define RAFEEQ_CONFIG_H

#include <string>
#include <map>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Config {
private:
    json config_data;
    std::string config_file;

public:
    Config();
    
    // Load configuration from file
    void loadFromFile(const std::string& file_path);
    
    // Get configuration values
    int getServerPort() const;
    std::string getDatabaseUrl() const;
    std::string getRedisUrl() const;
    std::string getGoogleMapsApiKey() const;
    bool isDevelopment() const;
    
    // Set configuration values
    void setServerPort(int port);
    void setDatabaseUrl(const std::string& url);
    void setRedisUrl(const std::string& url);
    
    // Get raw config
    json getConfig() const { return config_data; }
};

#endif // RAFEEQ_CONFIG_H
