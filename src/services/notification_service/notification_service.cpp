#include "services/notification_service/notification_service.h"
#include <iostream>
#include <algorithm>
#include <uuid/uuid.h>

NotificationService::NotificationService(const std::string& db_conn)
    : db_connection_string(db_conn) {}

void NotificationService::sendNotification(const Notification& notification) {
    notification_queue.push(notification);
    std::cout << "✓ Notification sent to " << notification.recipient_id 
              << ": " << notification.message << std::endl;
}

void NotificationService::sendPilgrimNotification(const std::string& pilgrim_id, 
                                                  const std::string& message,
                                                  NotificationType type) {
    Notification notif;
    notif.id = "notif_" + pilgrim_id + "_" + std::to_string(time(nullptr));
    notif.recipient_id = pilgrim_id;
    notif.recipient_type = "pilgrim";
    notif.type = type;
    notif.title = "تنبيه";
    notif.message = message;
    notif.is_read = false;
    notif.created_at = std::chrono::system_clock::now();
    
    sendNotification(notif);
}

void NotificationService::sendOperatorNotification(const std::string& operator_id,
                                                   const std::string& message,
                                                   NotificationType type) {
    Notification notif;
    notif.id = "notif_" + operator_id + "_" + std::to_string(time(nullptr));
    notif.recipient_id = operator_id;
    notif.recipient_type = "operator";
    notif.type = type;
    notif.title = "تنبيه";
    notif.message = message;
    notif.is_read = false;
    notif.created_at = std::chrono::system_clock::now();
    
    sendNotification(notif);
}

void NotificationService::createAlert(const Alert& alert) {
    active_alerts.push_back(alert);
    std::cout << "✓ Created alert: " << alert.type << " (severity: " << alert.severity << ")" << std::endl;
}

Alert NotificationService::getAlert(const std::string& alert_id) {
    for (const auto& alert : active_alerts) {
        if (alert.id == alert_id) {
            return alert;
        }
    }
    throw std::runtime_error("Alert not found: " + alert_id);
}

std::vector<Alert> NotificationService::getTripAlerts(const std::string& trip_id) {
    std::vector<Alert> result;
    for (const auto& alert : active_alerts) {
        if (alert.trip_id == trip_id) {
            result.push_back(alert);
        }
    }
    return result;
}

std::vector<Alert> NotificationService::getActiveAlerts() {
    std::vector<Alert> result;
    for (const auto& alert : active_alerts) {
        if (!alert.is_resolved) {
            result.push_back(alert);
        }
    }
    return result;
}

void NotificationService::resolveAlert(const std::string& alert_id) {
    for (auto& alert : active_alerts) {
        if (alert.id == alert_id) {
            alert.is_resolved = true;
            std::cout << "✓ Resolved alert: " << alert_id << std::endl;
            return;
        }
    }
}

std::vector<Notification> NotificationService::getPilgrimNotifications(const std::string& pilgrim_id) {
    std::vector<Notification> result;
    // Would typically fetch from database
    return result;
}

std::vector<Notification> NotificationService::getUnreadNotifications(const std::string& recipient_id) {
    std::vector<Notification> result;
    // Would typically fetch from database
    return result;
}

void NotificationService::markAsRead(const std::string& notification_id) {
    std::cout << "✓ Marked notification as read: " << notification_id << std::endl;
}

void NotificationService::alertRouteDeviation(const std::string& vehicle_id, 
                                             const std::string& trip_id,
                                             double deviation_distance_km) {
    Alert alert;
    alert.id = "alert_" + vehicle_id + "_" + std::to_string(time(nullptr));
    alert.trip_id = trip_id;
    alert.vehicle_id = vehicle_id;
    alert.type = "deviation";
    alert.severity = deviation_distance_km > 5.0 ? "high" : "medium";
    alert.description = "الحافلة انحرفت عن المسار بمسافة " + std::to_string(static_cast<int>(deviation_distance_km)) + " كم";
    alert.is_resolved = false;
    alert.triggered_at = std::chrono::system_clock::now();
    
    createAlert(alert);
    sendOperatorNotification(trip_id, alert.description, NotificationType::ROUTE_DEVIATION);
}

void NotificationService::alertPilgrimLost(const std::string& pilgrim_id,
                                          const std::string& trip_id,
                                          double latitude, double longitude) {
    Alert alert;
    alert.id = "alert_" + pilgrim_id + "_lost_" + std::to_string(time(nullptr));
    alert.trip_id = trip_id;
    alert.type = "pilgrim_lost";
    alert.severity = "critical";
    alert.description = "الحاج غير متاح في الموقع المتوقع";
    alert.is_resolved = false;
    alert.triggered_at = std::chrono::system_clock::now();
    
    createAlert(alert);
    
    json location_data;
    location_data["latitude"] = latitude;
    location_data["longitude"] = longitude;
    
    sendOperatorNotification(trip_id, "تنبيه: حاج فقد - " + pilgrim_id, NotificationType::PILGRIM_LOST);
}

void NotificationService::alertEmergency(const std::string& trip_id, const std::string& message) {
    Alert alert;
    alert.id = "alert_emergency_" + trip_id + "_" + std::to_string(time(nullptr));
    alert.trip_id = trip_id;
    alert.type = "emergency";
    alert.severity = "critical";
    alert.description = message;
    alert.is_resolved = false;
    alert.triggered_at = std::chrono::system_clock::now();
    
    createAlert(alert);
    sendOperatorNotification(trip_id, "تنبيه طوارئ: " + message, NotificationType::EMERGENCY);
}

void NotificationService::alertNearAccommodation(const std::string& pilgrim_id,
                                                const std::string& accommodation_name) {
    sendPilgrimNotification(pilgrim_id, 
                           "أنت قريب من فندقك: " + accommodation_name, 
                           NotificationType::ACCOMMODATION_NEAR);
}

int NotificationService::getActiveAlertCount() {
    return getActiveAlerts().size();
}

int NotificationService::getCriticalAlertCount() {
    int count = 0;
    for (const auto& alert : active_alerts) {
        if (!alert.is_resolved && alert.severity == "critical") {
            count++;
        }
    }
    return count;
}

json NotificationService::exportAlertsAsJson(const std::string& trip_id) {
    json alerts_json = json::object();
    auto alerts = getTripAlerts(trip_id);
    
    alerts_json["total_alerts"] = alerts.size();
    alerts_json["alerts"] = json::array();
    
    for (const auto& alert : alerts) {
        json alert_json;
        alert_json["id"] = alert.id;
        alert_json["type"] = alert.type;
        alert_json["severity"] = alert.severity;
        alert_json["description"] = alert.description;
        alert_json["is_resolved"] = alert.is_resolved;
        alerts_json["alerts"].push_back(alert_json);
    }
    
    return alerts_json;
}
