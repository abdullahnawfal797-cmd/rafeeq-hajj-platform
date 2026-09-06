#include "services/geofence_service/geofence_service.h"
#include <cmath>
#include <iostream>
#include <algorithm>

const double EARTH_RADIUS_KM = 6371.0;

double haversineDistanceGeo(double lat1, double lon1, double lat2, double lon2) {
    double dLat = (lat2 - lat1) * M_PI / 180.0;
    double dLon = (lon2 - lon1) * M_PI / 180.0;
    
    double a = sin(dLat / 2) * sin(dLat / 2) +
               cos(lat1 * M_PI / 180.0) * cos(lat2 * M_PI / 180.0) *
               sin(dLon / 2) * sin(dLon / 2);
    
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    return EARTH_RADIUS_KM * c;
}

GeofenceService::GeofenceService(const std::string& db_conn)
    : db_connection_string(db_conn) {}

void GeofenceService::createGeofence(const GeoFence& geofence) {
    geofences[geofence.id] = geofence;
    std::cout << "✓ Created geofence: " << geofence.name << " (type: " << geofence.type << ")" << std::endl;
}

GeoFence GeofenceService::getGeofence(const std::string& geofence_id) {
    auto it = geofences.find(geofence_id);
    if (it == geofences.end()) {
        throw std::runtime_error("Geofence not found: " + geofence_id);
    }
    return it->second;
}

std::vector<GeoFence> GeofenceService::getAllGeofences() {
    std::vector<GeoFence> result;
    for (const auto& [id, fence] : geofences) {
        result.push_back(fence);
    }
    return result;
}

std::vector<GeoFence> GeofenceService::getZoneGeofences(const std::string& zone_type) {
    std::vector<GeoFence> result;
    for (const auto& [id, fence] : geofences) {
        if (fence.zone_type == zone_type) {
            result.push_back(fence);
        }
    }
    return result;
}

void GeofenceService::updateGeofence(const GeoFence& geofence) {
    if (geofences.find(geofence.id) != geofences.end()) {
        geofences[geofence.id] = geofence;
        std::cout << "✓ Updated geofence: " << geofence.id << std::endl;
    }
}

void GeofenceService::deleteGeofence(const std::string& geofence_id) {
    if (geofences.erase(geofence_id) > 0) {
        std::cout << "✓ Deleted geofence: " << geofence_id << std::endl;
    }
}

bool GeofenceService::isPointInGeofence(double latitude, double longitude, const std::string& geofence_id) {
    try {
        const GeoFence& fence = getGeofence(geofence_id);
        
        if (fence.type == "circle") {
            double distance = haversineDistanceGeo(latitude, longitude, fence.latitude, fence.longitude);
            return distance <= fence.radius_km;
        } else if (fence.type == "polygon") {
            return isPointInPolygon(latitude, longitude, fence.polygon_points);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error checking geofence: " << e.what() << std::endl;
    }
    
    return false;
}

bool GeofenceService::isPointInAnyGeofence(double latitude, double longitude) {
    for (const auto& [id, fence] : geofences) {
        if (isPointInGeofence(latitude, longitude, id)) {
            return true;
        }
    }
    return false;
}

std::vector<GeoFence> GeofenceService::getGeofencesAtPoint(double latitude, double longitude) {
    std::vector<GeoFence> result;
    for (const auto& [id, fence] : geofences) {
        if (isPointInGeofence(latitude, longitude, id)) {
            result.push_back(fence);
        }
    }
    return result;
}

void GeofenceService::recordGeofenceEvent(const GeofenceEvent& event) {
    geofence_events.push_back(event);
    std::cout << "✓ Recorded geofence event: " << event.entity_id << " - " << event.event_type << std::endl;
}

std::vector<GeofenceEvent> GeofenceService::getEntityEvents(const std::string& entity_id) {
    std::vector<GeofenceEvent> result;
    for (const auto& event : geofence_events) {
        if (event.entity_id == entity_id) {
            result.push_back(event);
        }
    }
    return result;
}

std::vector<GeofenceEvent> GeofenceService::getUnprocessedEvents() {
    std::vector<GeofenceEvent> result;
    for (const auto& event : geofence_events) {
        if (!event.processed) {
            result.push_back(event);
        }
    }
    return result;
}

void GeofenceService::markEventProcessed(const std::string& event_id) {
    for (auto& event : geofence_events) {
        if (event.id == event_id) {
            event.processed = true;
            return;
        }
    }
}

bool GeofenceService::isInProhibitedZone(double latitude, double longitude) {
    auto prohibited = getZoneGeofences("prohibited");
    for (const auto& zone : prohibited) {
        if (isPointInGeofence(latitude, longitude, zone.id)) {
            return true;
        }
    }
    return false;
}

bool GeofenceService::isNearAccommodation(double latitude, double longitude, const std::string& accommodation_id) {
    try {
        const GeoFence& acc_fence = getGeofence(accommodation_id);
        return isPointInGeofence(latitude, longitude, accommodation_id);
    } catch (...) {
        return false;
    }
}

std::vector<GeoFence> GeofenceService::getNearbyAccommodations(double latitude, double longitude, double radius_km) {
    std::vector<GeoFence> result;
    auto accommodations = getZoneGeofences("accommodation");
    
    for (const auto& acc : accommodations) {
        double distance = haversineDistanceGeo(latitude, longitude, acc.latitude, acc.longitude);
        if (distance <= radius_km) {
            result.push_back(acc);
        }
    }
    
    // Sort by distance
    std::sort(result.begin(), result.end(),
             [latitude, longitude](const GeoFence& a, const GeoFence& b) {
                 double dist_a = haversineDistanceGeo(latitude, longitude, a.latitude, a.longitude);
                 double dist_b = haversineDistanceGeo(latitude, longitude, b.latitude, b.longitude);
                 return dist_a < dist_b;
             });
    
    return result;
}

bool GeofenceService::isNearMosque(double latitude, double longitude) {
    auto mosques = getZoneGeofences("mosque");
    for (const auto& mosque : mosques) {
        double distance = haversineDistanceGeo(latitude, longitude, mosque.latitude, mosque.longitude);
        if (distance <= 0.5) {  // 500 meters
            return true;
        }
    }
    return false;
}

bool GeofenceService::isPointInPolygon(double latitude, double longitude, 
                                      const std::vector<std::pair<double, double>>& polygon) {
    // Ray casting algorithm
    int n = polygon.size();
    if (n < 3) return false;
    
    bool inside = false;
    for (int i = 0, j = n - 1; i < n; j = i++) {
        double xi = polygon[i].first, yi = polygon[i].second;
        double xj = polygon[j].first, yj = polygon[j].second;
        
        bool intersect = ((yi > longitude) != (yj > longitude)) &&
                        (latitude < (xj - xi) * (longitude - yi) / (yj - yi) + xi);
        if (intersect) inside = !inside;
    }
    
    return inside;
}

double GeofenceService::distanceToGeofence(double latitude, double longitude, const std::string& geofence_id) {
    try {
        const GeoFence& fence = getGeofence(geofence_id);
        double distance = haversineDistanceGeo(latitude, longitude, fence.latitude, fence.longitude);
        return std::max(0.0, distance - fence.radius_km);
    } catch (...) {
        return -1.0;
    }
}

json GeofenceService::exportGeofenceAsJson(const std::string& geofence_id) {
    json fence_json = json::object();
    
    try {
        const GeoFence& fence = getGeofence(geofence_id);
        
        fence_json["id"] = fence.id;
        fence_json["name"] = fence.name;
        fence_json["type"] = fence.type;
        fence_json["zone_type"] = fence.zone_type;
        fence_json["latitude"] = fence.latitude;
        fence_json["longitude"] = fence.longitude;
        fence_json["radius_km"] = fence.radius_km;
        
    } catch (const std::exception& e) {
        fence_json["error"] = e.what();
    }
    
    return fence_json;
}

json GeofenceService::exportAllGeofencesAsJson() {
    json fences_json = json::array();
    
    for (const auto& [id, fence] : geofences) {
        fences_json.push_back(exportGeofenceAsJson(id));
    }
    
    return fences_json;
}
