#ifndef RAFEEQ_MODELS_H
#define RAFEEQ_MODELS_H

#include <string>
#include <vector>
#include <map>
#include <nlohmann/json.hpp>
#include <chrono>

using json = nlohmann::json;

// ==================== Tenant Model ====================
struct Tenant {
    std::string id;
    std::string name;
    std::string email;
    std::string phone;
    std::string country;
    std::string address;
    std::string status;  // active, inactive, suspended
    std::string subscription_tier;  // free, basic, professional, enterprise
    std::chrono::system_clock::time_point created_at;
    std::chrono::system_clock::time_point updated_at;
    
    json toJson() const {
        json j;
        j["id"] = id;
        j["name"] = name;
        j["email"] = email;
        j["phone"] = phone;
        j["country"] = country;
        j["status"] = status;
        j["subscription_tier"] = subscription_tier;
        return j;
    }
};

// ==================== User Model ====================
struct User {
    std::string id;
    std::string tenant_id;
    std::string username;
    std::string email;
    std::string password_hash;
    std::string role;  // admin, operator, guide, pilgrim
    std::string status;  // active, inactive, suspended
    std::string phone;
    std::string full_name;
    std::chrono::system_clock::time_point created_at;
    
    json toJson() const {
        json j;
        j["id"] = id;
        j["tenant_id"] = tenant_id;
        j["username"] = username;
        j["email"] = email;
        j["role"] = role;
        j["status"] = status;
        j["full_name"] = full_name;
        return j;
    }
};

// ==================== Vehicle Model ====================
struct Vehicle {
    std::string id;
    std::string tenant_id;
    std::string registration_number;
    std::string vehicle_type;  // bus, minibus, van, car
    std::string driver_id;
    int capacity;
    std::string status;  // active, maintenance, inactive
    double latitude;
    double longitude;
    std::chrono::system_clock::time_point last_gps_update;
    
    json toJson() const {
        json j;
        j["id"] = id;
        j["registration_number"] = registration_number;
        j["vehicle_type"] = vehicle_type;
        j["driver_id"] = driver_id;
        j["capacity"] = capacity;
        j["status"] = status;
        j["latitude"] = latitude;
        j["longitude"] = longitude;
        return j;
    }
};

// ==================== Response Models ====================
struct APIResponse {
    bool success;
    int status_code;
    std::string message;
    json data;
    
    json toJson() const {
        json j;
        j["success"] = success;
        j["status_code"] = status_code;
        j["message"] = message;
        j["data"] = data;
        j["timestamp"] = std::to_string(std::time(nullptr));
        return j;
    }
};

struct PaginatedResponse {
    int page;
    int per_page;
    int total;
    int total_pages;
    json data;
    
    json toJson() const {
        json j;
        j["page"] = page;
        j["per_page"] = per_page;
        j["total"] = total;
        j["total_pages"] = total_pages;
        j["data"] = data;
        return j;
    }
};

#endif // RAFEEQ_MODELS_H
