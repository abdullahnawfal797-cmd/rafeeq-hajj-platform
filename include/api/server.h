#ifndef RAFEEQ_API_SERVER_H
#define RAFEEQ_API_SERVER_H

#include <string>
#include <memory>
#include <vector>
#include <nlohmann/json.hpp>
#include "services/gps_service/gps_service.h"
#include "services/location_service/location_service.h"
#include "services/trip_service/trip_service.h"
#include "services/notification_service/notification_service.h"
#include "services/geofence_service/geofence_service.h"
#include "core/database/connection_pool.h"

using json = nlohmann::json;

class APIServer {
private:
    int port;
    bool running;
    
    // Services
    std::shared_ptr<GPSService> gps_service;
    std::shared_ptr<LocationService> location_service;
    std::shared_ptr<TripService> trip_service;
    std::shared_ptr<NotificationService> notification_service;
    std::shared_ptr<GeofenceService> geofence_service;
    std::shared_ptr<ConnectionPool> db_pool;

public:
    APIServer(int server_port);
    ~APIServer();
    
    // Server lifecycle
    void start();
    void stop();
    void run();
    
    // Service getters
    std::shared_ptr<GPSService> getGPSService() { return gps_service; }
    std::shared_ptr<LocationService> getLocationService() { return location_service; }
    std::shared_ptr<TripService> getTripService() { return trip_service; }
    std::shared_ptr<NotificationService> getNotificationService() { return notification_service; }
    std::shared_ptr<GeofenceService> getGeofenceService() { return geofence_service; }
    
    // Initialize services
    void initializeServices();
    
    // Health check
    json healthCheck();
};

#endif // RAFEEQ_API_SERVER_H
