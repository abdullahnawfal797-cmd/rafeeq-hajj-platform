#include "api/middleware/auth_middleware.h"
#include <iostream>
#include <ctime>
#include <sstream>
#include <iomanip>

AuthMiddleware::AuthMiddleware(const std::string& secret)
    : jwt_secret(secret) {}

AuthToken AuthMiddleware::authenticate(const std::string& username, const std::string& password) {
    // This would typically verify against database
    // For now, we create a test token
    
    AuthToken token;
    token.user_id = username;
    token.tenant_id = "default-tenant";
    token.role = "operator";
    token.expires_at = std::time(nullptr) + 86400;  // 24 hours
    token.token = generateJWT(token.user_id, token.tenant_id, token.role, 86400);
    
    tokens_cache[token.token] = token;
    
    std::cout << "✓ User authenticated: " << username << std::endl;
    return token;
}

bool AuthMiddleware::validateToken(const std::string& token) {
    auto it = tokens_cache.find(token);
    if (it == tokens_cache.end()) {
        return false;
    }
    
    long long now = std::time(nullptr);
    return it->second.expires_at > now;
}

AuthToken AuthMiddleware::getTokenInfo(const std::string& token) {
    auto it = tokens_cache.find(token);
    if (it == tokens_cache.end()) {
        throw std::runtime_error("Token not found");
    }
    return it->second;
}

void AuthMiddleware::revokeToken(const std::string& token) {
    tokens_cache.erase(token);
    std::cout << "✓ Token revoked" << std::endl;
}

std::string AuthMiddleware::generateJWT(const std::string& user_id, const std::string& tenant_id,
                                       const std::string& role, long long expires_in_seconds) {
    // Simple JWT-like token generation (simplified version)
    std::ostringstream token_stream;
    long long now = std::time(nullptr);
    long long exp = now + expires_in_seconds;
    
    token_stream << user_id << "." << tenant_id << "." << role << "." << exp;
    return token_stream.str();
}

bool AuthMiddleware::verifyJWT(const std::string& token) {
    return validateToken(token);
}
