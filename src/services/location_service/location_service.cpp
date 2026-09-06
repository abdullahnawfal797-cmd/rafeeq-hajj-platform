#include "services/location_service/location_service.h"
#include <cmath>
#include <iostream>
#include <sstream>
#include <iomanip>

const double EARTH_RADIUS_KM = 6371.0;

double haversineDistanceLoc(double lat1, double lon1, double lat2, double lon2) {
    double dLat = (lat2 - lat1) * M_PI / 180.0;
    double dLon = (lon2 - lon1) * M_PI / 180.0;
    
    double a = sin(dLat / 2) * sin(dLat / 2) +
               cos(lat1 * M_PI / 180.0) * cos(lat2 * M_PI / 180.0) *
               sin(dLon / 2) * sin(dLon / 2);
    
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    return EARTH_RADIUS_KM * c;
}

LocationService::LocationService(const std::string& db_conn, const std::string& maps_key)
    : db_connection_string(db_conn), google_maps_api_key(maps_key) {}

void LocationService::registerAccommodation(const Accommodation& accommodation) {
    accommodations_cache[accommodation.id] = accommodation;
    std::cout << "✓ Registered accommodation: " << accommodation.name 
              << " at (" << accommodation.latitude << ", " << accommodation.longitude << ")" << std::endl;
}

Accommodation LocationService::getAccommodation(const std::string& accommodation_id) {
    auto it = accommodations_cache.find(accommodation_id);
    if (it == accommodations_cache.end()) {
        throw std::runtime_error("Accommodation not found: " + accommodation_id);
    }
    return it->second;
}

std::vector<Accommodation> LocationService::getTenantAccommodations(const std::string& tenant_id) {
    std::vector<Accommodation> result;
    for (const auto& [id, acc] : accommodations_cache) {
        if (acc.tenant_id == tenant_id) {
            result.push_back(acc);
        }
    }
    return result;
}

void LocationService::updatePilgrimLocation(const PilgrimLocation& location) {
    std::cout << "✓ Updated location for pilgrim " << location.pilgrim_id 
              << " at (" << location.latitude << ", " << location.longitude << ")" << std::endl;
}

PilgrimLocation LocationService::getPilgrimLocation(const std::string& pilgrim_id) {
    // This would typically fetch from database
    // For now, return a placeholder
    PilgrimLocation loc;
    loc.pilgrim_id = pilgrim_id;
    return loc;
}

Accommodation LocationService::getPilgrimAccommodation(const std::string& pilgrim_id) {
    // Fetch from database: SELECT accommodation FROM pilgrim_assignments WHERE pilgrim_id = ?
    // For now, throw error
    throw std::runtime_error("Pilgrim accommodation not found");
}

std::vector<NearbyPlace> LocationService::findNearbyPlaces(double latitude, double longitude,
                                                           const std::string& place_type,
                                                           double radius_km) {
    std::vector<NearbyPlace> nearby;
    
    // This would typically call Google Places API or use local data
    // For demonstration, return some known places
    
    NearbyPlace place1;
    place1.name = "مسجد أم القرى";
    place1.type = "mosque";
    place1.latitude = latitude + 0.01;
    place1.longitude = longitude + 0.01;
    place1.distance_km = haversineDistanceLoc(latitude, longitude, place1.latitude, place1.longitude);
    place1.description = "مسجد قريب من مكانك";
    
    if (place_type.empty() || place_type == "mosque") {
        nearby.push_back(place1);
    }
    
    return nearby;
}

double LocationService::getDistanceToAccommodation(const std::string& pilgrim_id) {
    try {
        PilgrimLocation pilgrim_loc = getPilgrimLocation(pilgrim_id);
        Accommodation accommodation = getPilgrimAccommodation(pilgrim_id);
        
        return haversineDistanceLoc(pilgrim_loc.latitude, pilgrim_loc.longitude,
                                   accommodation.latitude, accommodation.longitude);
    } catch (const std::exception& e) {
        return -1.0;
    }
}

bool LocationService::isPilgrimNearAccommodation(const std::string& pilgrim_id, double radius_meters) {
    double distance_km = getDistanceToAccommodation(pilgrim_id);
    double radius_km = radius_meters / 1000.0;
    return distance_km >= 0 && distance_km <= radius_km;
}

std::string LocationService::getNavigationUrl(double current_lat, double current_lon,
                                             const std::string& accommodation_id) {
    try {
        const Accommodation& acc = getAccommodation(accommodation_id);
        
        std::ostringstream url;
        url << "https://www.google.com/maps/dir/"
            << std::fixed << std::setprecision(6)
            << current_lat << "," << current_lon << "/"
            << acc.latitude << "," << acc.longitude;
        
        return url.str();
    } catch (const std::exception& e) {
        return "";
    }
}

json LocationService::exportLocationAsJson(const std::string& pilgrim_id) {
    json location_json = json::object();
    location_json["pilgrim_id"] = pilgrim_id;
    
    try {
        PilgrimLocation pilgrim_loc = getPilgrimLocation(pilgrim_id);
        Accommodation accommodation = getPilgrimAccommodation(pilgrim_id);
        
        location_json["current_location"] = json::object();
        location_json["current_location"]["latitude"] = pilgrim_loc.latitude;
        location_json["current_location"]["longitude"] = pilgrim_loc.longitude;
        location_json["current_location"]["timestamp"] = pilgrim_loc.timestamp;
        
        location_json["accommodation"] = json::object();
        location_json["accommodation"]["name"] = accommodation.name;
        location_json["accommodation"]["latitude"] = accommodation.latitude;
        location_json["accommodation"]["longitude"] = accommodation.longitude;
        location_json["accommodation"]["address"] = accommodation.address;
        location_json["accommodation"]["phone"] = accommodation.phone;
        
        double distance = getDistanceToAccommodation(pilgrim_id);
        location_json["distance_to_accommodation_km"] = distance;
        
    } catch (const std::exception& e) {
        location_json["error"] = e.what();
    }
    
    return location_json;
}

json LocationService::getAccommodationWithNearby(const std::string& accommodation_id) {
    json acc_json = json::object();
    
    try {
        const Accommodation& acc = getAccommodation(accommodation_id);
        
        acc_json["id"] = acc.id;
        acc_json["name"] = acc.name;
        acc_json["type"] = acc.type;
        acc_json["latitude"] = acc.latitude;
        acc_json["longitude"] = acc.longitude;
        acc_json["address"] = acc.address;
        acc_json["phone"] = acc.phone;
        
        auto nearby_places = findNearbyPlaces(acc.latitude, acc.longitude, "", 2.0);
        acc_json["nearby_places"] = json::array();
        
        for (const auto& place : nearby_places) {
            json place_json = json::object();
            place_json["name"] = place.name;
            place_json["type"] = place.type;
            place_json["distance_km"] = place.distance_km;
            place_json["description"] = place.description;
            acc_json["nearby_places"].push_back(place_json);
        }
        
    } catch (const std::exception& e) {
        acc_json["error"] = e.what();
    }
    
    return acc_json;
}
