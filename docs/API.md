# API Reference - منصة رفيق الحاج والمعتمر

## 🔑 المصادقة

### Login

```http
POST /api/auth/login
Content-Type: application/json

{
  "username": "admin",
  "password": "password123"
}

# Response
{
  "success": true,
  "data": {
    "token": "eyJhbGciOiJIUzI1NiIs...",
    "user": {
      "id": "user_123",
      "username": "admin",
      "role": "operator"
    },
    "expires_in": 86400
  }
}
```

### Logout

```http
POST /api/auth/logout
Authorization: Bearer TOKEN

# Response
{
  "success": true,
  "message": "Logged out successfully"
}
```

## 🚗 GPS Tracking

### Record Vehicle Location

```http
POST /api/gps/location
Authorization: Bearer TOKEN
Content-Type: application/json

{
  "vehicle_id": "vehicle_001",
  "trip_id": "trip_001",
  "latitude": 21.4225,
  "longitude": 39.8262,
  "speed": 60.5,
  "heading": 180
}

# Response
{
  "success": true,
  "data": {
    "location_id": "loc_123",
    "recorded_at": "2026-09-06T20:30:00Z"
  }
}
```

### Get Current Vehicle Location

```http
GET /api/gps/vehicles/{vehicle_id}/current
Authorization: Bearer TOKEN

# Response
{
  "success": true,
  "data": {
    "vehicle_id": "vehicle_001",
    "latitude": 21.4225,
    "longitude": 39.8262,
    "speed": 60.5,
    "heading": 180,
    "timestamp": "2026-09-06T20:30:00Z"
  }
}
```

### Get Vehicle Trail

```http
GET /api/gps/vehicles/{vehicle_id}/trail?start_time=...&end_time=...
Authorization: Bearer TOKEN

# Response
{
  "success": true,
  "data": {
    "vehicle_id": "vehicle_001",
    "total_distance_km": 45.3,
    "locations": [
      {
        "latitude": 21.4225,
        "longitude": 39.8262,
        "speed": 60.5,
        "timestamp": "2026-09-06T20:30:00Z"
      }
    ]
  }
}
```

## 🧳 Location Services (Pilgrims)

### Get Pilgrim Accommodation

```http
GET /api/location/pilgrims/{pilgrim_id}/accommodation
Authorization: Bearer TOKEN

# Response
{
  "success": true,
  "data": {
    "accommodation_id": "acc_001",
    "name": "فندق الراية",
    "type": "hotel",
    "latitude": 21.4245,
    "longitude": 39.8275,
    "address": "شارع العنترة، مكة المكرمة",
    "phone": "+966123456789",
    "distance_km": 2.5,
    "navigation_url": "https://maps.google.com/..."
  }
}
```

### Update Pilgrim Location

```http
POST /api/location/pilgrims/{pilgrim_id}/update
Authorization: Bearer TOKEN
Content-Type: application/json

{
  "latitude": 21.4200,
  "longitude": 39.8250
}

# Response
{
  "success": true,
  "data": {
    "pilgrim_id": "pilgrim_001",
    "location_updated": true,
    "near_accommodation": true
  }
}
```

### Find Nearby Places

```http
GET /api/location/nearby?latitude=21.4200&longitude=39.8250&type=mosque&radius=2.0
Authorization: Bearer TOKEN

# Response
{
  "success": true,
  "data": {
    "nearby_places": [
      {
        "name": "المسجد الحرام",
        "type": "mosque",
        "distance_km": 0.8,
        "latitude": 21.4225,
        "longitude": 39.8262
      }
    ]
  }
}
```

## 📍 Geofences

### Create Geofence

```http
POST /api/geofence
Authorization: Bearer TOKEN
Content-Type: application/json

{
  "name": "فندق الراية",
  "type": "circle",
  "latitude": 21.4245,
  "longitude": 39.8275,
  "radius_km": 0.5,
  "zone_type": "accommodation",
  "alert_on_enter": true,
  "alert_on_exit": false
}

# Response
{
  "success": true,
  "data": {
    "geofence_id": "geo_001",
    "created_at": "2026-09-06T20:30:00Z"
  }
}
```

### Check Point in Geofence

```http
GET /api/geofence/{geofence_id}/check?latitude=21.4245&longitude=39.8275
Authorization: Bearer TOKEN

# Response
{
  "success": true,
  "data": {
    "geofence_id": "geo_001",
    "is_inside": true,
    "distance_km": 0.1
  }
}
```

## 🚁 Trips Management

### Create Trip

```http
POST /api/trips
Authorization: Bearer TOKEN
Content-Type: application/json

{
  "name": "رحلة الحج 2026",
  "departure_location": "الرياض",
  "destination": "مكة المكرمة",
  "start_date": "2026-09-10T06:00:00Z",
  "end_date": "2026-09-20T23:00:00Z"
}

# Response
{
  "success": true,
  "data": {
    "trip_id": "trip_001",
    "name": "رحلة الحج 2026",
    "status": "planned",
    "created_at": "2026-09-06T20:30:00Z"
  }
}
```

### Get Trip Details

```http
GET /api/trips/{trip_id}
Authorization: Bearer TOKEN

# Response
{
  "success": true,
  "data": {
    "id": "trip_001",
    "name": "رحلة الحج 2026",
    "status": "active",
    "total_pilgrims": 150,
    "active_pilgrims": 148,
    "lost_pilgrims": 2,
    "groups": [
      {
        "id": "group_001",
        "name": "المجموعة الأولى",
        "pilgrim_count": 50
      }
    ]
  }
}
```

## 🔔 Notifications

### Get Notifications

```http
GET /api/notifications?unread=true
Authorization: Bearer TOKEN

# Response
{
  "success": true,
  "data": {
    "notifications": [
      {
        "id": "notif_001",
        "type": "ROUTE_DEVIATION",
        "title": "انحراف المسار",
        "message": "الحافلة رقم 001 انحرفت عن المسار",
        "created_at": "2026-09-06T20:30:00Z",
        "is_read": false
      }
    ]
  }
}
```

### Mark as Read

```http
PUT /api/notifications/{notification_id}/read
Authorization: Bearer TOKEN

# Response
{
  "success": true
}
```

## ⚠️ Alerts

### Get Trip Alerts

```http
GET /api/alerts?trip_id={trip_id}&severity=critical
Authorization: Bearer TOKEN

# Response
{
  "success": true,
  "data": {
    "total_alerts": 3,
    "alerts": [
      {
        "id": "alert_001",
        "type": "pilgrim_lost",
        "severity": "critical",
        "description": "حاج فقد في موقع جدة",
        "is_resolved": false,
        "triggered_at": "2026-09-06T20:30:00Z"
      }
    ]
  }
}
```

## 🏥 Health Check

```http
GET /api/health

# Response
{
  "status": "healthy",
  "timestamp": "2026-09-06T20:30:00Z",
  "services": {
    "gps": "ok",
    "location": "ok",
    "trip": "ok",
    "notification": "ok",
    "geofence": "ok"
  }
}
```

---

## Status Codes

- `200 OK` - نجح
- `201 Created` - تم الإنشاء
- `400 Bad Request` - طلب غير صحيح
- `401 Unauthorized` - غير مصرح
- `403 Forbidden` - ممنوع
- `404 Not Found` - غير موجود
- `500 Server Error` - خطأ في الخادم

---

**آخر تحديث:** 2026-09-06
