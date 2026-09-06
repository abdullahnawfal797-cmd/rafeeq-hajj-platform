#ifndef RAFEEQ_GPS_SERVICE_H
#define RAFEEQ_GPS_SERVICE_H

#include <string>
#include <vector>
#include <memory>
#include <nlohmann/json.hpp>
#include <chrono>

using json = nlohmann::json;

struct GPSLocation {
    double latitude;
    double longitude;
    double speed;  // km/h
    double heading;  // 0-360 degrees
    std::chrono::system_clock::time_point timestamp;
    std::string vehicle_id;
    std::string trip_id;
};

struct GPSTrail {
    std::string vehicle_id;
    std::vector<GPSLocation> locations;
    double total_distance;  // km
    std::string status;  // 'active', 'stopped', 'delayed'
};

class GPSService {
private:
    std::string redis_url;
    std::string db_connection_string;
    std::map<std::string, GPSTrail> active_trails;

public:
    GPSService(const std::string& redis_url, const std::string& db_conn);
    
    // Record GPS location
    void recordLocation(const GPSLocation& location);
    
    // Get current location of vehicle
    GPSLocation getCurrentLocation(const std::string& vehicle_id);
    
    // Get complete trail for a vehicle
    GPSTrail getTrail(const std::string& vehicle_id);
    
    // Get trail for specific trip
    GPSTrail getTripsTrail(const std::string& trip_id);
    
    // Check if vehicle deviated from route
    bool checkRouteDeviation(const std::string& vehicle_id, const std::vector<GPSLocation>& planned_route);
    
    // Calculate distance traveled
    double calculateDistance(const GPSLocation& loc1, const GPSLocation& loc2);
    
    // Get all active vehicles
    std::vector<std::string> getActiveVehicles();
    
    // Start tracking a vehicle
    void startTracking(const std::string& vehicle_id, const std::string& trip_id);
    
    // Stop tracking a vehicle
    void stopTracking(const std::string& vehicle_id);
    
    // Get ETA to destination
    int getEstimatedTimeToArrival(const std::string& vehicle_id, const GPSLocation& destination);
    
    // Export trail to JSON
    json exportTrailAsJson(const std::string& vehicle_id);
};

#endif // RAFEEQ_GPS_SERVICE_H
