#include "core/multi_tenancy/tenant_context.h"
#include <iostream>
#include <stdexcept>

thread_local std::string TenantContext::current_tenant_id;
std::mutex TenantContext::tenant_mutex;
std::map<std::string, std::string> TenantContext::tenant_databases;

void TenantContext::setCurrentTenant(const std::string& tenant_id) {
    current_tenant_id = tenant_id;
    std::cout << "✓ Set tenant context: " << tenant_id << std::endl;
}

std::string TenantContext::getCurrentTenant() {
    if (current_tenant_id.empty()) {
        throw std::runtime_error("No tenant context set");
    }
    return current_tenant_id;
}

void TenantContext::registerTenantDatabase(const std::string& tenant_id, const std::string& db_url) {
    std::lock_guard<std::mutex> lock(tenant_mutex);
    tenant_databases[tenant_id] = db_url;
    std::cout << "✓ Registered database for tenant: " << tenant_id << std::endl;
}

std::string TenantContext::getTenantDatabase(const std::string& tenant_id) {
    std::lock_guard<std::mutex> lock(tenant_mutex);
    auto it = tenant_databases.find(tenant_id);
    if (it == tenant_databases.end()) {
        throw std::runtime_error("No database configured for tenant: " + tenant_id);
    }
    return it->second;
}

bool TenantContext::validateTenantAccess(const std::string& tenant_id, const std::string& user_id) {
    // This would typically check against database
    // For now, we assume valid if tenant is registered
    return tenant_databases.find(tenant_id) != tenant_databases.end();
}

void TenantContext::clear() {
    current_tenant_id = "";
}
