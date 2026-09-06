#ifndef RAFEEQ_DATABASE_CONNECTION_H
#define RAFEEQ_DATABASE_CONNECTION_H

#include <string>
#include <memory>
#include <libpq-fe.h>

class DatabaseConnection {
private:
    PGconn* connection;
    std::string connection_string;
    bool connected;

public:
    DatabaseConnection(const std::string& conn_str);
    ~DatabaseConnection();
    
    // Connection management
    void connect();
    void disconnect();
    bool isConnected() const { return connected; }
    
    // Query execution
    PGresult* executeQuery(const std::string& query);
    bool executeUpdate(const std::string& query);
    
    // Transaction management
    void beginTransaction();
    void commit();
    void rollback();
    
    // Get raw connection
    PGconn* getConnection() const { return connection; }
};

#endif // RAFEEQ_DATABASE_CONNECTION_H
