#ifndef RAFEEQ_MULTI_TENANCY_H
#define RAFEEQ_MULTI_TENANCY_H

#include <string>
#include <map>
#include <memory>
#include <mutex>

class TenantContext {
private:
    thread_local static std::string current_tenant_id;
    static std::mutex tenant_mutex;
    static std::map<std::string, std::string> tenant_databases;

public:
    // Set tenant context for current thread
    static void setCurrentTenant(const std::string& tenant_id);
    
    // Get current tenant
    static std::string getCurrentTenant();
    
    // Register tenant database
    static void registerTenantDatabase(const std::string& tenant_id, const std::string& db_url);
    
    // Get tenant database
    static std::string getTenantDatabase(const std::string& tenant_id);
    
    // Validate tenant access
    static bool validateTenantAccess(const std::string& tenant_id, const std::string& user_id);
    
    // Clear context
    static void clear();
};

#endif // RAFEEQ_MULTI_TENANCY_H
