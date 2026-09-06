# 🕌 منصة رفيق الحاج والمعتمر

منصة تشغيلية تجارية **SaaS متعددة المستأجرين** لإدارة رحلات الحج والعمرة بكفاءة عالية.

## 📋 المحتويات الرئيسية

### 1. **لوحة التحكم (Dashboard)**
- إدارة الرحلات والمجموعات
- تسيير الحجاج والمعتمرين
- إدارة التسكين والنقل
- نظام التنبيهات والطوارئ
- التقارير والتحليلات

### 2. **نظام GPS المتقدم**

#### 🚗 GPS للمتعهد/السائق (Real-time Tracking)
- تتبع مباشر للحافلة على الخريطة
- عرض مسار الرحلة الكامل (Breadcrumb Trail)
- حساب السرعة والاتجاه
- وقت الوصول المتوقع (ETA)
- تنبيهات الانحراف عن المسار
- لوحة موحدة لجميع الحافلات من مكتب واحد

#### 🧳 GPS للحاج/المعتمر (Location Helper)
- تحديد موقع الفندق/السكن على الخريطة
- حساب المسافة والمسار للفندق
- تنبيهات قرب الموقع
- مشاركة آمنة للموقع مع المرشد
- قائمة الأماكن المهمة بالقرب
- ميزة "اين فندقي" عند الضياع

### 3. **تطبيق الحاج/المعتمر**
- تطبيق بسيط وخفيف الوزن
- معلومات الرحلة الشخصية
- التنبيهات والتحديثات الفورية
- تزامن بيانات من المنصة الأم

## 🏗️ البنية المعمارية

### Stack التقني:
- **اللغة:** C++17/20
- **Backend:** C++ REST Framework (crow/beast)
- **قاعدة البيانات:** PostgreSQL (Multi-tenant)
- **نظام GPS:** Google Maps API / OpenStreetMap
- **Real-time:** WebSockets
- **تخزين البيانات:** Redis (Caching)
- **التطبيق الميداني:** Flutter/React Native

### البنية العامة:

```
rafeeq-hajj-platform/
├── src/
│   ├── core/                    # المنطق الأساسي
│   │   ├── database/           # إدارة قاعدة البيانات
│   │   ├── models/             # نماذج البيانات
│   │   └── config/             # الإعدادات
│   ├── services/               # الخدمات الرئيسية
│   │   ├── gps_service/        # خدمة GPS
│   │   ├── location_service/   # خدمة الموقع
│   │   ├── trip_service/       # خدمة الرحلات
│   │   ├── notification_service/ # خدمة التنبيهات
│   │   └── geofence_service/   # خدمة المناطق الجغرافية
│   ├── api/                    # واجهات REST API
│   │   ├── handlers/
│   │   ├── middleware/
│   │   └── validators/
│   └── main.cpp               # نقطة البداية
├── tests/                      # اختبارات الوحدة والتكامل
├── config/                     # ملفات الإعدادات
├── docker/                     # Docker configuration
├── docs/                       # التوثيق
└── CMakeLists.txt             # نظام البناء

```

## 📦 المتطلبات

- C++17 أو أحدث
- CMake 3.15+
- PostgreSQL 12+
- Redis 6+
- Google Maps API Key (اختياري)

## 🚀 البدء السريع

```bash
# Clone المستودع
git clone https://github.com/abdullahnawfal797-cmd/rafeeq-hajj-platform.git
cd rafeeq-hajj-platform

# بناء المشروع
mkdir build && cd build
cmake ..
make

# تشغيل الخادم
./rafeeq_server
```

## 📚 التوثيق

- [معمارية النظام](docs/ARCHITECTURE.md)
- [دليل الإعداد](docs/SETUP.md)
- [API Reference](docs/API.md)
- [GPS System](docs/GPS_SYSTEM.md)

## 👥 الفريق

تم الإنشاء بواسطة: Abdullah Nawfal

## 📝 الترخيص

MIT License

---

**حالة المشروع:** 🔨 تحت التطوير النشط
