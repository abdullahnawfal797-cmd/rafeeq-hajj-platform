#ifndef RAFEEQ_TRIP_SERVICE_H
#define RAFEEQ_TRIP_SERVICE_H

#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include <memory>
#include <chrono>

using json = nlohmann::json;

struct RouteWaypoint {
    double latitude;
    double longitude;
    std::string name;
    std::string description;
    int order;  // Sequence order
};

struct Trip {
    std::string id;
    std::string name;
    std::string tenant_id;
    std::string status;  // planned, active, completed, cancelled
    std::string departure_location;
    std::string destination;
    std::chrono::system_clock::time_point start_date;
    std::chrono::system_clock::time_point end_date;
    std::vector<RouteWaypoint> route_waypoints;
    std::vector<std::string> vehicle_ids;
    int total_pilgrims;
};

struct Group {
    std::string id;
    std::string trip_id;
    std::string name;
    std::string vehicle_id;
    int pilgrim_count;
    std::vector<std::string> pilgrim_ids;
    std::string guide_id;
    std::string status;  // active, completed
};

struct Pilgrim {
    std::string id;
    std::string trip_id;
    std::string group_id;
    std::string accommodation_id;
    std::string name;
    std::string phone;
    std::string passport_number;
    std::string status;  // active, completed, lost, found
};

class TripService {
private:
    std::string db_connection_string;
    std::map<std::string, Trip> trips_cache;
    std::map<std::string, Group> groups_cache;
    std::map<std::string, Pilgrim> pilgrims_cache;

public:
    TripService(const std::string& db_conn);
    
    // Trip Management
    void createTrip(const Trip& trip);
    Trip getTrip(const std::string& trip_id);
    std::vector<Trip> getTenantTrips(const std::string& tenant_id);
    void updateTripStatus(const std::string& trip_id, const std::string& status);
    void addRouteWaypoint(const std::string& trip_id, const RouteWaypoint& waypoint);
    
    // Group Management
    void createGroup(const Group& group);
    Group getGroup(const std::string& group_id);
    std::vector<Group> getTripGroups(const std::string& trip_id);
    void updateGroupStatus(const std::string& group_id, const std::string& status);
    
    // Pilgrim Management
    void addPilgrim(const Pilgrim& pilgrim);
    Pilgrim getPilgrim(const std::string& pilgrim_id);
    std::vector<Pilgrim> getGroupPilgrims(const std::string& group_id);
    std::vector<Pilgrim> getTripPilgrims(const std::string& trip_id);
    void updatePilgrimStatus(const std::string& pilgrim_id, const std::string& status);
    void assignPilgrimToGroup(const std::string& pilgrim_id, const std::string& group_id);
    void assignAccommodation(const std::string& pilgrim_id, const std::string& accommodation_id);
    
    // Trip Statistics
    int getTotalPilgrimsInTrip(const std::string& trip_id);
    int getActivePilgrimsInTrip(const std::string& trip_id);
    int getLostPilgrimsInTrip(const std::string& trip_id);
    
    // Export functions
    json exportTripAsJson(const std::string& trip_id);
    json exportGroupAsJson(const std::string& group_id);
};

#endif // RAFEEQ_TRIP_SERVICE_H
