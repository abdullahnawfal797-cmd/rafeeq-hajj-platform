#ifndef RAFEEQ_GEOFENCE_SERVICE_H
#define RAFEEQ_GEOFENCE_SERVICE_H

#include <string>
#include <vector>
#include <map>
#include <nlohmann/json.hpp>
#include <memory>

using json = nlohmann::json;

struct GeoFence {
    std::string id;
    std::string name;
    std::string type;  // circle, polygon, accommodation
    double latitude;   // Center for circle
    double longitude;  // Center for circle
    double radius_km;  // For circle type
    std::vector<std::pair<double, double>> polygon_points;  // For polygon type
    std::string zone_type;  // accommodation, prohibited, important, waypoint
    bool alert_on_enter;
    bool alert_on_exit;
};

struct GeofenceEvent {
    std::string id;
    std::string geofence_id;
    std::string entity_id;  // vehicle_id or pilgrim_id
    std::string entity_type;  // vehicle, pilgrim
    std::string event_type;  // enter, exit
    double latitude;
    double longitude;
    std::string timestamp;
    bool processed;
};

class GeofenceService {
private:
    std::string db_connection_string;
    std::map<std::string, GeoFence> geofences;
    std::vector<GeofenceEvent> geofence_events;

public:
    GeofenceService(const std::string& db_conn);
    
    // Geofence Management
    void createGeofence(const GeoFence& geofence);
    GeoFence getGeofence(const std::string& geofence_id);
    std::vector<GeoFence> getAllGeofences();
    std::vector<GeoFence> getZoneGeofences(const std::string& zone_type);
    void updateGeofence(const GeoFence& geofence);
    void deleteGeofence(const std::string& geofence_id);
    
    // Geofence Checking
    bool isPointInGeofence(double latitude, double longitude, const std::string& geofence_id);
    bool isPointInAnyGeofence(double latitude, double longitude);
    std::vector<GeoFence> getGeofencesAtPoint(double latitude, double longitude);
    
    // Event Management
    void recordGeofenceEvent(const GeofenceEvent& event);
    std::vector<GeofenceEvent> getEntityEvents(const std::string& entity_id);
    std::vector<GeofenceEvent> getUnprocessedEvents();
    void markEventProcessed(const std::string& event_id);
    
    // Specific Zone Checks
    bool isInProhibitedZone(double latitude, double longitude);
    bool isNearAccommodation(double latitude, double longitude, const std::string& accommodation_id);
    std::vector<GeoFence> getNearbyAccommodations(double latitude, double longitude, double radius_km = 5.0);
    bool isNearMosque(double latitude, double longitude);
    
    // Polygon check (point in polygon)
    bool isPointInPolygon(double latitude, double longitude, const std::vector<std::pair<double, double>>& polygon);
    
    // Distance calculation
    double distanceToGeofence(double latitude, double longitude, const std::string& geofence_id);
    
    // Export functions
    json exportGeofenceAsJson(const std::string& geofence_id);
    json exportAllGeofencesAsJson();
};

#endif // RAFEEQ_GEOFENCE_SERVICE_H
