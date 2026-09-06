#ifndef RAFEEQ_NOTIFICATION_SERVICE_H
#define RAFEEQ_NOTIFICATION_SERVICE_H

#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include <queue>
#include <memory>
#include <chrono>

using json = nlohmann::json;

enum class NotificationType {
    ROUTE_DEVIATION,
    PILGRIM_LOCATION_ALERT,
    VEHICLE_DELAY,
    PILGRIM_LOST,
    PILGRIM_FOUND,
    ACCOMMODATION_NEAR,
    EMERGENCY,
    GENERAL_INFO
};

struct Notification {
    std::string id;
    std::string recipient_id;
    std::string recipient_type;  // pilgrim, operator, guide
    NotificationType type;
    std::string title;
    std::string message;
    std::string trip_id;
    std::string vehicle_id;
    bool is_read;
    std::chrono::system_clock::time_point created_at;
    json data;  // Additional data (location, etc.)
};

struct Alert {
    std::string id;
    std::string trip_id;
    std::string vehicle_id;
    std::string type;  // deviation, delay, emergency
    std::string severity;  // low, medium, high, critical
    std::string description;
    bool is_resolved;
    std::chrono::system_clock::time_point triggered_at;
};

class NotificationService {
private:
    std::queue<Notification> notification_queue;
    std::vector<Alert> active_alerts;
    std::string db_connection_string;

public:
    NotificationService(const std::string& db_conn);
    
    // Send notifications
    void sendNotification(const Notification& notification);
    void sendPilgrimNotification(const std::string& pilgrim_id, const std::string& message, 
                                 NotificationType type = NotificationType::GENERAL_INFO);
    void sendOperatorNotification(const std::string& operator_id, const std::string& message,
                                  NotificationType type = NotificationType::GENERAL_INFO);
    
    // Alert management
    void createAlert(const Alert& alert);
    Alert getAlert(const std::string& alert_id);
    std::vector<Alert> getTripAlerts(const std::string& trip_id);
    std::vector<Alert> getActiveAlerts();
    void resolveAlert(const std::string& alert_id);
    
    // Notification retrieval
    std::vector<Notification> getPilgrimNotifications(const std::string& pilgrim_id);
    std::vector<Notification> getUnreadNotifications(const std::string& recipient_id);
    void markAsRead(const std::string& notification_id);
    
    // Route deviation alerts
    void alertRouteDeviation(const std::string& vehicle_id, const std::string& trip_id,
                           double deviation_distance_km);
    
    // Pilgrim lost alert
    void alertPilgrimLost(const std::string& pilgrim_id, const std::string& trip_id,
                         double latitude, double longitude);
    
    // Emergency alert
    void alertEmergency(const std::string& trip_id, const std::string& message);
    
    // Location-based alert (pilgrim near accommodation)
    void alertNearAccommodation(const std::string& pilgrim_id, const std::string& accommodation_name);
    
    // Get alert statistics
    int getActiveAlertCount();
    int getCriticalAlertCount();
    
    // Export functions
    json exportAlertsAsJson(const std::string& trip_id);
};

#endif // RAFEEQ_NOTIFICATION_SERVICE_H
