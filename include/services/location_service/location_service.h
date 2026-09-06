#ifndef RAFEEQ_LOCATION_SERVICE_H
#define RAFEEQ_LOCATION_SERVICE_H

#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include <memory>

using json = nlohmann::json;

struct Accommodation {
    std::string id;
    std::string name;
    std::string type;  // hotel, apartment, etc.
    double latitude;
    double longitude;
    std::string address;
    std::string phone;
    std::string tenant_id;
};

struct PilgrimLocation {
    std::string pilgrim_id;
    std::string accommodation_id;
    double latitude;
    double longitude;
    std::string timestamp;
};

struct NearbyPlace {
    std::string name;
    std::string type;  // mosque, market, hospital, etc.
    double latitude;
    double longitude;
    double distance_km;
    std::string description;
};

class LocationService {
private:
    std::string db_connection_string;
    std::string google_maps_api_key;
    std::map<std::string, Accommodation> accommodations_cache;

public:
    LocationService(const std::string& db_conn, const std::string& maps_key = "");
    
    // Register accommodation (hotel, apartment, etc.)
    void registerAccommodation(const Accommodation& accommodation);
    
    // Get accommodation by ID
    Accommodation getAccommodation(const std::string& accommodation_id);
    
    // Get all accommodations for a tenant
    std::vector<Accommodation> getTenantAccommodations(const std::string& tenant_id);
    
    // Update pilgrim location
    void updatePilgrimLocation(const PilgrimLocation& location);
    
    // Get pilgrim current location
    PilgrimLocation getPilgrimLocation(const std::string& pilgrim_id);
    
    // Get accommodation location for pilgrim
    Accommodation getPilgrimAccommodation(const std::string& pilgrim_id);
    
    // Find nearby places (mosques, markets, hospitals, etc.)
    std::vector<NearbyPlace> findNearbyPlaces(double latitude, double longitude, 
                                              const std::string& place_type = "", 
                                              double radius_km = 2.0);
    
    // Calculate distance to accommodation
    double getDistanceToAccommodation(const std::string& pilgrim_id);
    
    // Check if pilgrim is near accommodation
    bool isPilgrimNearAccommodation(const std::string& pilgrim_id, double radius_meters = 500.0);
    
    // Get navigation URL to accommodation
    std::string getNavigationUrl(double current_lat, double current_lon, 
                                const std::string& accommodation_id);
    
    // Export location as JSON
    json exportLocationAsJson(const std::string& pilgrim_id);
    
    // Get accommodation details with nearby places
    json getAccommodationWithNearby(const std::string& accommodation_id);
};

#endif // RAFEEQ_LOCATION_SERVICE_H
