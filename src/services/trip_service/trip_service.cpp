#include "services/trip_service/trip_service.h"
#include <iostream>
#include <algorithm>

TripService::TripService(const std::string& db_conn)
    : db_connection_string(db_conn) {}

void TripService::createTrip(const Trip& trip) {
    trips_cache[trip.id] = trip;
    std::cout << "✓ Created trip: " << trip.name << " (ID: " << trip.id << ")" << std::endl;
}

Trip TripService::getTrip(const std::string& trip_id) {
    auto it = trips_cache.find(trip_id);
    if (it == trips_cache.end()) {
        throw std::runtime_error("Trip not found: " + trip_id);
    }
    return it->second;
}

std::vector<Trip> TripService::getTenantTrips(const std::string& tenant_id) {
    std::vector<Trip> result;
    for (const auto& [id, trip] : trips_cache) {
        if (trip.tenant_id == tenant_id) {
            result.push_back(trip);
        }
    }
    return result;
}

void TripService::updateTripStatus(const std::string& trip_id, const std::string& status) {
    auto it = trips_cache.find(trip_id);
    if (it != trips_cache.end()) {
        it->second.status = status;
        std::cout << "✓ Updated trip status: " << trip_id << " -> " << status << std::endl;
    }
}

void TripService::addRouteWaypoint(const std::string& trip_id, const RouteWaypoint& waypoint) {
    auto it = trips_cache.find(trip_id);
    if (it != trips_cache.end()) {
        it->second.route_waypoints.push_back(waypoint);
        std::cout << "✓ Added waypoint: " << waypoint.name << " to trip " << trip_id << std::endl;
    }
}

void TripService::createGroup(const Group& group) {
    groups_cache[group.id] = group;
    std::cout << "✓ Created group: " << group.name << " (ID: " << group.id << ")" << std::endl;
}

Group TripService::getGroup(const std::string& group_id) {
    auto it = groups_cache.find(group_id);
    if (it == groups_cache.end()) {
        throw std::runtime_error("Group not found: " + group_id);
    }
    return it->second;
}

std::vector<Group> TripService::getTripGroups(const std::string& trip_id) {
    std::vector<Group> result;
    for (const auto& [id, group] : groups_cache) {
        if (group.trip_id == trip_id) {
            result.push_back(group);
        }
    }
    return result;
}

void TripService::updateGroupStatus(const std::string& group_id, const std::string& status) {
    auto it = groups_cache.find(group_id);
    if (it != groups_cache.end()) {
        it->second.status = status;
        std::cout << "✓ Updated group status: " << group_id << " -> " << status << std::endl;
    }
}

void TripService::addPilgrim(const Pilgrim& pilgrim) {
    pilgrims_cache[pilgrim.id] = pilgrim;
    std::cout << "✓ Added pilgrim: " << pilgrim.name << " (ID: " << pilgrim.id << ")" << std::endl;
}

Pilgrim TripService::getPilgrim(const std::string& pilgrim_id) {
    auto it = pilgrims_cache.find(pilgrim_id);
    if (it == pilgrims_cache.end()) {
        throw std::runtime_error("Pilgrim not found: " + pilgrim_id);
    }
    return it->second;
}

std::vector<Pilgrim> TripService::getGroupPilgrims(const std::string& group_id) {
    std::vector<Pilgrim> result;
    for (const auto& [id, pilgrim] : pilgrims_cache) {
        if (pilgrim.group_id == group_id) {
            result.push_back(pilgrim);
        }
    }
    return result;
}

std::vector<Pilgrim> TripService::getTripPilgrims(const std::string& trip_id) {
    std::vector<Pilgrim> result;
    for (const auto& [id, pilgrim] : pilgrims_cache) {
        if (pilgrim.trip_id == trip_id) {
            result.push_back(pilgrim);
        }
    }
    return result;
}

void TripService::updatePilgrimStatus(const std::string& pilgrim_id, const std::string& status) {
    auto it = pilgrims_cache.find(pilgrim_id);
    if (it != pilgrims_cache.end()) {
        it->second.status = status;
        std::cout << "✓ Updated pilgrim status: " << pilgrim_id << " -> " << status << std::endl;
    }
}

void TripService::assignPilgrimToGroup(const std::string& pilgrim_id, const std::string& group_id) {
    auto pilgrim_it = pilgrims_cache.find(pilgrim_id);
    auto group_it = groups_cache.find(group_id);
    
    if (pilgrim_it != pilgrims_cache.end() && group_it != groups_cache.end()) {
        pilgrim_it->second.group_id = group_id;
        group_it->second.pilgrim_ids.push_back(pilgrim_id);
        group_it->second.pilgrim_count++;
        std::cout << "✓ Assigned pilgrim " << pilgrim_id << " to group " << group_id << std::endl;
    }
}

void TripService::assignAccommodation(const std::string& pilgrim_id, const std::string& accommodation_id) {
    auto it = pilgrims_cache.find(pilgrim_id);
    if (it != pilgrims_cache.end()) {
        it->second.accommodation_id = accommodation_id;
        std::cout << "✓ Assigned accommodation " << accommodation_id << " to pilgrim " << pilgrim_id << std::endl;
    }
}

int TripService::getTotalPilgrimsInTrip(const std::string& trip_id) {
    auto pilgrims = getTripPilgrims(trip_id);
    return pilgrims.size();
}

int TripService::getActivePilgrimsInTrip(const std::string& trip_id) {
    auto pilgrims = getTripPilgrims(trip_id);
    return std::count_if(pilgrims.begin(), pilgrims.end(),
                        [](const Pilgrim& p) { return p.status == "active"; });
}

int TripService::getLostPilgrimsInTrip(const std::string& trip_id) {
    auto pilgrims = getTripPilgrims(trip_id);
    return std::count_if(pilgrims.begin(), pilgrims.end(),
                        [](const Pilgrim& p) { return p.status == "lost"; });
}

json TripService::exportTripAsJson(const std::string& trip_id) {
    json trip_json = json::object();
    
    try {
        const Trip& trip = getTrip(trip_id);
        
        trip_json["id"] = trip.id;
        trip_json["name"] = trip.name;
        trip_json["status"] = trip.status;
        trip_json["departure_location"] = trip.departure_location;
        trip_json["destination"] = trip.destination;
        trip_json["total_pilgrims"] = trip.total_pilgrims;
        
        trip_json["statistics"] = json::object();
        trip_json["statistics"]["active_pilgrims"] = getActivePilgrimsInTrip(trip_id);
        trip_json["statistics"]["lost_pilgrims"] = getLostPilgrimsInTrip(trip_id);
        
        // Add groups
        auto groups = getTripGroups(trip_id);
        trip_json["groups"] = json::array();
        for (const auto& group : groups) {
            json group_json;
            group_json["id"] = group.id;
            group_json["name"] = group.name;
            group_json["pilgrim_count"] = group.pilgrim_count;
            trip_json["groups"].push_back(group_json);
        }
        
    } catch (const std::exception& e) {
        trip_json["error"] = e.what();
    }
    
    return trip_json;
}

json TripService::exportGroupAsJson(const std::string& group_id) {
    json group_json = json::object();
    
    try {
        const Group& group = getGroup(group_id);
        
        group_json["id"] = group.id;
        group_json["name"] = group.name;
        group_json["trip_id"] = group.trip_id;
        group_json["vehicle_id"] = group.vehicle_id;
        group_json["pilgrim_count"] = group.pilgrim_count;
        group_json["status"] = group.status;
        
        // Add pilgrims
        auto pilgrims = getGroupPilgrims(group_id);
        group_json["pilgrims"] = json::array();
        for (const auto& pilgrim : pilgrims) {
            json pilgrim_json;
            pilgrim_json["id"] = pilgrim.id;
            pilgrim_json["name"] = pilgrim.name;
            pilgrim_json["status"] = pilgrim.status;
            group_json["pilgrims"].push_back(pilgrim_json);
        }
        
    } catch (const std::exception& e) {
        group_json["error"] = e.what();
    }
    
    return group_json;
}
