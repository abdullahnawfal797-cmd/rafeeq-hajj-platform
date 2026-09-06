#ifndef RAFEEQ_CONNECTION_POOL_H
#define RAFEEQ_CONNECTION_POOL_H

#include "database_connection.h"
#include <queue>
#include <mutex>
#include <memory>

class ConnectionPool {
private:
    std::queue<std::shared_ptr<DatabaseConnection>> available_connections;
    std::queue<std::shared_ptr<DatabaseConnection>> used_connections;
    std::mutex pool_mutex;
    std::string connection_string;
    int pool_size;
    int max_connections;

public:
    ConnectionPool(const std::string& conn_str, int size = 5, int max = 10);
    ~ConnectionPool();
    
    // Get a connection from the pool
    std::shared_ptr<DatabaseConnection> getConnection();
    
    // Return a connection to the pool
    void releaseConnection(std::shared_ptr<DatabaseConnection> conn);
    
    // Get pool statistics
    int getAvailableConnections() const;
    int getUsedConnections() const;
    
    // Initialize the pool
    void initialize();
    
    // Shutdown the pool
    void shutdown();
};

#endif // RAFEEQ_CONNECTION_POOL_H
