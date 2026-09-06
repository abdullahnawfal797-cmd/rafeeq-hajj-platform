#include "services/gps_service/gps_service.h"
#include <cmath>
#include <iostream>
#include <algorithm>

const double EARTH_RADIUS_KM = 6371.0;  // Earth's radius in kilometers

// Haversine formula to calculate distance between two coordinates
double haversineDistance(double lat1, double lon1, double lat2, double lon2) {
    double dLat = (lat2 - lat1) * M_PI / 180.0;
    double dLon = (lon2 - lon1) * M_PI / 180.0;
    
    double a = sin(dLat / 2) * sin(dLat / 2) +
               cos(lat1 * M_PI / 180.0) * cos(lat2 * M_PI / 180.0) *
               sin(dLon / 2) * sin(dLon / 2);
    
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    return EARTH_RADIUS_KM * c;
}

GPSService::GPSService(const std::string& redis_url, const std::string& db_conn)
    : redis_url(redis_url), db_connection_string(db_conn) {}

void GPSService::recordLocation(const GPSLocation& location) {
    // Store in Redis for real-time access
    // Also store in PostgreSQL for historical data
    
    auto it = active_trails.find(location.vehicle_id);
    if (it == active_trails.end()) {
        GPSTrail trail;
        trail.vehicle_id = location.vehicle_id;
        trail.total_distance = 0.0;
        trail.status = "active";
        active_trails[location.vehicle_id] = trail;
    }
    
    // Calculate distance from last location
    GPSTrail& trail = active_trails[location.vehicle_id];
    if (!trail.locations.empty()) {
        const GPSLocation& last = trail.locations.back();
        double dist = calculateDistance(last, location);
        trail.total_distance += dist;
    }
    
    trail.locations.push_back(location);
    
    std::cout << "✓ Location recorded for vehicle " << location.vehicle_id 
              << " at (" << location.latitude << ", " << location.longitude << ")" << std::endl;
}

GPSLocation GPSService::getCurrentLocation(const std::string& vehicle_id) {
    auto it = active_trails.find(vehicle_id);
    if (it == active_trails.end() || it->second.locations.empty()) {
        throw std::runtime_error("No location data for vehicle: " + vehicle_id);
    }
    
    return it->second.locations.back();
}

GPSTrail GPSService::getTrail(const std::string& vehicle_id) {
    auto it = active_trails.find(vehicle_id);
    if (it == active_trails.end()) {
        throw std::runtime_error("No trail data for vehicle: " + vehicle_id);
    }
    
    return it->second;
}

GPSTrail GPSService::getTripsTrail(const std::string& trip_id) {
    GPSTrail combined_trail;
    combined_trail.trip_id = trip_id;
    combined_trail.total_distance = 0.0;
    
    for (const auto& [vehicle_id, trail] : active_trails) {
        if (trail.locations.size() > 0 && trail.locations[0].trip_id == trip_id) {
            for (const auto& loc : trail.locations) {
                combined_trail.locations.push_back(loc);
            }
            combined_trail.total_distance += trail.total_distance;
        }
    }
    
    return combined_trail;
}

bool GPSService::checkRouteDeviation(const std::string& vehicle_id, const std::vector<GPSLocation>& planned_route) {
    if (planned_route.size() < 2) {
        return false;
    }
    
    auto it = active_trails.find(vehicle_id);
    if (it == active_trails.end() || it->second.locations.empty()) {
        return false;
    }
    
    const GPSLocation& current = it->second.locations.back();
    
    // Check distance to planned route (simplified: check last waypoint)
    const GPSLocation& last_waypoint = planned_route.back();
    double deviation = calculateDistance(current, last_waypoint);
    
    // If more than 1 km away from route, it's a deviation
    const double DEVIATION_THRESHOLD = 1.0;  // km
    return deviation > DEVIATION_THRESHOLD;
}

double GPSService::calculateDistance(const GPSLocation& loc1, const GPSLocation& loc2) {
    return haversineDistance(loc1.latitude, loc1.longitude, loc2.latitude, loc2.longitude);
}

std::vector<std::string> GPSService::getActiveVehicles() {
    std::vector<std::string> vehicles;
    for (const auto& [vehicle_id, trail] : active_trails) {
        if (trail.status == "active") {
            vehicles.push_back(vehicle_id);
        }
    }
    return vehicles;
}

void GPSService::startTracking(const std::string& vehicle_id, const std::string& trip_id) {
    GPSTrail trail;
    trail.vehicle_id = vehicle_id;
    trail.total_distance = 0.0;
    trail.status = "active";
    active_trails[vehicle_id] = trail;
    
    std::cout << "✓ Started tracking vehicle " << vehicle_id << " for trip " << trip_id << std::endl;
}

void GPSService::stopTracking(const std::string& vehicle_id) {
    auto it = active_trails.find(vehicle_id);
    if (it != active_trails.end()) {
        it->second.status = "stopped";
        std::cout << "✓ Stopped tracking vehicle " << vehicle_id << std::endl;
    }
}

int GPSService::getEstimatedTimeToArrival(const std::string& vehicle_id, const GPSLocation& destination) {
    try {
        const GPSLocation& current = getCurrentLocation(vehicle_id);
        double distance = calculateDistance(current, destination);
        
        // Assume average speed of 60 km/h
        const double AVERAGE_SPEED = 60.0;
        int eta_minutes = static_cast<int>((distance / AVERAGE_SPEED) * 60);
        
        return eta_minutes;
    } catch (const std::exception& e) {
        return -1;  // Unknown
    }
}

json GPSService::exportTrailAsJson(const std::string& vehicle_id) {
    try {
        const GPSTrail& trail = getTrail(vehicle_id);
        
        json trail_json = json::object();
        trail_json["vehicle_id"] = trail.vehicle_id;
        trail_json["total_distance_km"] = trail.total_distance;
        trail_json["status"] = trail.status;
        trail_json["locations"] = json::array();
        
        for (const auto& loc : trail.locations) {
            json loc_json = json::object();
            loc_json["latitude"] = loc.latitude;
            loc_json["longitude"] = loc.longitude;
            loc_json["speed_kmh"] = loc.speed;
            loc_json["heading"] = loc.heading;
            
            trail_json["locations"].push_back(loc_json);
        }
        
        return trail_json;
    } catch (const std::exception& e) {
        return json::object();
    }
}
