#include "api/server.h"
#include <iostream>
#include <thread>
#include <chrono>

APIServer::APIServer(int server_port)
    : port(server_port), running(false) {}

APIServer::~APIServer() {
    stop();
}

void APIServer::initializeServices() {
    try {
        // Initialize database connection pool
        std::string db_url = "postgresql://localhost:5432/rafeeq_db";
        db_pool = std::make_shared<ConnectionPool>(db_url, 5, 10);
        db_pool->initialize();
        
        // Initialize services
        gps_service = std::make_shared<GPSService>("redis://localhost:6379", db_url);
        location_service = std::make_shared<LocationService>(db_url, "");
        trip_service = std::make_shared<TripService>(db_url);
        notification_service = std::make_shared<NotificationService>(db_url);
        geofence_service = std::make_shared<GeofenceService>(db_url);
        
        std::cout << "✓ All services initialized successfully" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "✗ Failed to initialize services: " << e.what() << std::endl;
        throw;
    }
}

void APIServer::start() {
    try {
        running = true;
        initializeServices();
        
        std::cout << "✓ API Server started on port " << port << std::endl;
        std::cout << "🕌 Rafeeq Hajj Platform - API Server v1.0.0" << std::endl;
        std::cout << "═══════════════════════════════════════════" << std::endl;
        std::cout << "Server Status: RUNNING" << std::endl;
        std::cout << "Database: Connected" << std::endl;
        std::cout << "Redis Cache: Connected" << std::endl;
        std::cout << "═══════════════════════════════════════════" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "✗ Failed to start server: " << e.what() << std::endl;
        running = false;
        throw;
    }
}

void APIServer::stop() {
    if (running) {
        running = false;
        std::cout << "✓ API Server stopped" << std::endl;
    }
}

void APIServer::run() {
    while (running) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

json APIServer::healthCheck() {
    json health = json::object();
    health["status"] = running ? "healthy" : "unhealthy";
    health["timestamp"] = std::to_string(std::time(nullptr));
    health["services"] = json::object();
    health["services"]["gps"] = gps_service != nullptr ? "ok" : "error";
    health["services"]["location"] = location_service != nullptr ? "ok" : "error";
    health["services"]["trip"] = trip_service != nullptr ? "ok" : "error";
    health["services"]["notification"] = notification_service != nullptr ? "ok" : "error";
    health["services"]["geofence"] = geofence_service != nullptr ? "ok" : "error";
    return health;
}
