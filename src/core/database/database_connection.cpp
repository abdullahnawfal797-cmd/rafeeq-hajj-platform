#include "core/database/database_connection.h"
#include <iostream>
#include <stdexcept>

DatabaseConnection::DatabaseConnection(const std::string& conn_str)
    : connection(nullptr), connection_string(conn_str), connected(false) {}

DatabaseConnection::~DatabaseConnection() {
    disconnect();
}

void DatabaseConnection::connect() {
    connection = PQconnectdb(connection_string.c_str());
    
    if (PQstatus(connection) != CONNECTION_OK) {
        std::string error = PQerrorMessage(connection);
        PQfinish(connection);
        throw std::runtime_error("Database connection failed: " + error);
    }
    
    connected = true;
    std::cout << "✓ Connected to database" << std::endl;
}

void DatabaseConnection::disconnect() {
    if (connection) {
        PQfinish(connection);
        connection = nullptr;
        connected = false;
    }
}

PGresult* DatabaseConnection::executeQuery(const std::string& query) {
    if (!connected) {
        throw std::runtime_error("Database not connected");
    }
    
    PGresult* result = PQexec(connection, query.c_str());
    
    if (PQresultStatus(result) != PGRES_TUPLES_OK && 
        PQresultStatus(result) != PGRES_COMMAND_OK) {
        std::string error = PQerrorMessage(connection);
        PQclear(result);
        throw std::runtime_error("Query failed: " + error);
    }
    
    return result;
}

bool DatabaseConnection::executeUpdate(const std::string& query) {
    if (!connected) {
        throw std::runtime_error("Database not connected");
    }
    
    PGresult* result = PQexec(connection, query.c_str());
    bool success = (PQresultStatus(result) == PGRES_COMMAND_OK);
    
    if (!success) {
        std::cerr << "Update failed: " << PQerrorMessage(connection) << std::endl;
    }
    
    PQclear(result);
    return success;
}

void DatabaseConnection::beginTransaction() {
    executeUpdate("BEGIN TRANSACTION;");
}

void DatabaseConnection::commit() {
    executeUpdate("COMMIT;");
}

void DatabaseConnection::rollback() {
    executeUpdate("ROLLBACK;");
}
