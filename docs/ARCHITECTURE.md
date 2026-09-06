# معمارية منصة رفيق الحاج والمعتمر

## 📐 البنية العامة

```
┌─────────────────────────────────────────────────────────────────┐
│                      العملاء (Clients)                          │
├─────────────────────────────────────────────────────────────────┤
│  📱 تطبيق الحاج    │  🖥️ لوحة التحكم    │  🚗 تطبيق السائق    │
│   (Mobile App)    │  (Admin Dashboard)  │  (Driver App)       │
└────────┬───────────────────┬──────────────────────────┬─────────┘
         │                   │                          │
         └───────────────────┴──────────────────────────┘
                             │
         ┌───────────────────▼───────────────────┐
         │   REST API Gateway (Port 8080)        │
         │   - Authentication Middleware         │
         │   - Multi-Tenancy Context             │
         │   - Rate Limiting                     │
         └───────────────────┬───────────────────┘
                             │
    ┌────────────────────────┼────────────────────────┐
    │                        │                        │
    ▼                        ▼                        ▼
┌──────────────┐    ┌──────────────┐    ┌──────────────┐
│ GPS Service  │    │ Location     │    │ Trip Service │
│              │    │ Service      │    │              │
└──────────────┘    └──────────────┘    └──────────────┘
    │                        │                        │
    └────────────────────────┼────────────────────────┘
                             │
         ┌───────────────────┴───────────────────┐
         │                                       │
    ▼                                       ▼
┌──────────────────────┐         ┌──────────────────────┐
│ Notification Service │         │ Geofence Service     │
│                      │         │                      │
└──────────────────────┘         └──────────────────────┘
    │                                       │
    │          ┌──────────────────────┐    │
    │          │                      │    │
    ▼          ▼                      ▼    ▼
┌────────────────────────────────────────────────┐
│          Database Layer (Multi-Tenant)        │
├────────────────────────────────────────────────┤
│  PostgreSQL Connection Pool                    │
│  - Tenant Isolation                            │
│  - Data Segregation                            │
└────────────────────────────────────────────────┘
    │                                       │
    ▼                                       ▼
┌──────────────────┐         ┌──────────────────────┐
│  PostgreSQL DB   │         │   Redis Cache        │
│  (Main Data)     │         │  (Real-time Data)    │
└──────────────────┘         └──────────────────────┘
```

## 🔄 تدفق البيانات

### 1. **تتبع الحافلة (Vehicle Tracking)**

```
السائق (Driver)
    │
    ├─ يرسل GPS Location
    │
    ▼
GPS Service
    │
    ├─ حفظ في Redis (Real-time)
    ├─ حفظ في PostgreSQL (History)
    ��─ فحص الانحراف عن المسار
    └─ فحص Geofences
         │
         ├─ إذا انحرف → Alert إلى Operator
         ├─ إذا اقترب من نقطة → Update Waypoint
         └─ إذا دخل Zone → Trigger Event
    │
    ▼
Operator Dashboard
    │
    └─ عرض الموقع الفوري على الخريطة
```

### 2. **تحديد الحاج لموقع الفندق (Pilgrim Location)**

```
الحاج (Pilgrim)
    │
    ├─ يضغط "أين فندقي؟"
    │
    ▼
Location Service
    │
    ├─ الحصول على موقع الفندق من DB
    ├─ حساب المسافة الحالية
    ├─ التحقق من Geofence (قرب الفندق)
    ├─ البحث عن أماكن قريبة
    │
    ▼
Pilgrim Mobile App
    │
    ├─ عرض خريطة الفندق
    ├─ حساب المسافة والمسار
    ├─ تنبيهات قرب الفندق
    └─ معلومات الاتصال والعنوان
```

### 3. **نظام التنبيهات (Alert System)**

```
Monitoring Thread
    │
    ├─ فحص انحراف الحافلة
    ├─ فحص تأخر الرحلة
    ├─ فحص دخول مناطق محظورة
    ├─ فحص فقدان الحاج
    │
    ▼
Notification Service
    │
    ├─ إنشاء تنبيه
    ├─ إضافة إلى قائمة التنبيهات
    ├─ إرسال Push Notification
    │
    ▼
Operator / Pilgrim
    │
    └─ تلقي التنبيه الفوري
```

## 🏗️ طبقات المعمارية

### 1. **Presentation Layer**
- تطبيقات العملاء (Mobile/Web)
- واجهات المستخدم

### 2. **API Layer**
- REST Endpoints
- Authentication
- Request Validation
- Response Formatting

### 3. **Service Layer**
- GPS Service
- Location Service
- Trip Service
- Notification Service
- Geofence Service

### 4. **Data Layer**
- Database Abstraction
- Connection Pool
- Multi-Tenancy Context
- Query Builders

### 5. **Infrastructure Layer**
- PostgreSQL
- Redis
- Message Queue (Future)
- External APIs (Google Maps, etc.)

## 🔐 الأمان

### Multi-Tenancy Isolation

```cpp
// كل طلب يحمل معرف المستأجر
TenantContext::setCurrentTenant("tenant_001");

// جميع الاستعلامات محدودة لهذا المستأجر
WHERE tenant_id = 'tenant_001'
```

### Authentication & Authorization

```cpp
// JWT Token
token = AuthMiddleware::generateJWT(
    user_id="user_123",
    tenant_id="tenant_001",
    role="operator",
    expires_in_seconds=86400
);

// Verification
bool valid = AuthMiddleware::verifyJWT(token);
```

## 📊 النسخ الاحتياطي والاستعادة

```bash
# نسخ احتياطي من PostgreSQL
pg_dump -U rafeeq_user rafeeq_db > backup.sql

# استعادة من نسخة احتياطية
psql -U rafeeq_user rafeeq_db < backup.sql
```

## 📈 قابلية التوسع

### Database Sharding (المستقبل)
- تقسيم البيانات حسب المستأجرين
- كل مستأجر كبير له قاعدة بيانات منفصلة

### Caching Strategy
- Redis للبيانات الفورية (GPS, Notifications)
- PostgreSQL للبيانات التاريخية والدائمة

### Message Queue (المستقبل)
- RabbitMQ/Kafka للمعالجة غير المتزامنة
- تقليل زمن استجابة API

## 🔌 Integration Points

### Google Maps API
- حساب المسافات والمسارات
- البحث عن أماكن قريبة
- التحقق من المسارات

### FCM/APNs
- إرسال Push Notifications
- تنبيهات فورية للهاتف

### SMS Gateway
- تنبيهات حرجة via SMS
- رسائل تأكيد

---

**ملاحظة:** هذه المعمارية قابلة للتطور والتعديل حسب احتياجات المشروع المستقبلية.
