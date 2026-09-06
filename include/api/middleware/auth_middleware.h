#ifndef RAFEEQ_AUTH_MIDDLEWARE_H
#define RAFEEQ_AUTH_MIDDLEWARE_H

#include <string>
#include <map>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct AuthToken {
    std::string token;
    std::string user_id;
    std::string tenant_id;
    std::string role;
    long long expires_at;
};

class AuthMiddleware {
private:
    std::map<std::string, AuthToken> tokens_cache;
    std::string jwt_secret;

public:
    AuthMiddleware(const std::string& secret);
    
    // Authenticate user and generate token
    AuthToken authenticate(const std::string& username, const std::string& password);
    
    // Validate token
    bool validateToken(const std::string& token);
    
    // Get token info
    AuthToken getTokenInfo(const std::string& token);
    
    // Revoke token
    void revokeToken(const std::string& token);
    
    // Generate JWT token
    std::string generateJWT(const std::string& user_id, const std::string& tenant_id,
                           const std::string& role, long long expires_in_seconds);
    
    // Verify JWT token
    bool verifyJWT(const std::string& token);
};

#endif // RAFEEQ_AUTH_MIDDLEWARE_H
