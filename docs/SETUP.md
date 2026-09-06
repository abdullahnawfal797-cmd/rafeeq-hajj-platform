# منصة رفيق الحاج والمعتمر - دليل البدء السريع

## 📋 المتطلبات

- C++17 أو أحدث
- CMake 3.15+
- Docker و Docker Compose
- PostgreSQL 12+ (أو استخدام Docker)
- Redis 6+ (أو استخدام Docker)
- Git

## 🚀 الخطوات الأولية

### 1. استنساخ المستودع

```bash
git clone https://github.com/abdullahnawfal797-cmd/rafeeq-hajj-platform.git
cd rafeeq-hajj-platform
```

### 2. إعداد المتغيرات البيئية

```bash
cp config/config.json.example config/config.json
# عدّل config/config.json بالقيم المناسبة
```

### 3. تشغيل مع Docker (الطريقة الموصى بها)

```bash
# بناء وتشغيل جميع الخدمات
docker-compose -f docker/docker-compose.yml up -d

# عرض السجلات
docker-compose -f docker/docker-compose.yml logs -f api

# إيقاف الخدمات
docker-compose -f docker/docker-compose.yml down
```

### 4. البناء اليدوي (بدون Docker)

```bash
# إنشاء مجلد البناء
mkdir -p build
cd build

# تشغيل CMake
cmake ..

# بناء المشروع
make -j$(nproc)

# تشغيل الخادم
./rafeeq_server
```

## 🔧 الإعدادات

### قاعدة البيانات

```bash
# إذا كنت تستخدم PostgreSQL محليًا
psql -U postgres -c "CREATE DATABASE rafeeq_db;"
psql -U postgres -d rafeeq_db -f config/database.sql
```

### Redis

يجب أن يكون Redis قيد التشغيل على `redis://localhost:6379`

## 📝 الاختبار

### فحص صحة الخادم

```bash
curl http://localhost:8080/api/health
```

### تسجيل الدخول

```bash
curl -X POST http://localhost:8080/api/auth/login \
  -H "Content-Type: application/json" \
  -d '{"username": "admin", "password": "admin123"}'
```

## 🗂️ هيكل المشروع

```
rafeeq-hajj-platform/
├── src/                    # الكود المصدري
│   ├── core/              # المنطق الأساسي
│   ├── services/          # الخدمات
│   ├── api/               # واجهات REST
│   └── main.cpp           # نقطة البداية
├── include/               # ملفات الرؤوس
├── tests/                 # اختبارات الوحدة
├── config/                # ملفات الإعدادات
├── docker/                # ملفات Docker
├── docs/                  # التوثيق
└── CMakeLists.txt        # نظام البناء
```

## 📚 التوثيق الإضافية

- [API Reference](docs/API.md)
- [معمارية النظام](docs/ARCHITECTURE.md)
- [نظام GPS](docs/GPS_SYSTEM.md)
- [الإدارة متعددة المستأجرين](docs/MULTI_TENANCY.md)

## 🐛 استكشاف الأخطاء

### المشاكل الشائعة

#### الاتصال برفض قاعدة البيانات

```bash
# تأكد من أن PostgreSQL يعمل
psql -U rafeeq_user -h localhost -d rafeeq_db
```

#### Redis غير متصل

```bash
# التحقق من Redis
redis-cli ping
# يجب أن يرد PONG
```

#### خطأ في البناء (CMake)

```bash
# امسح مجلد البناء وحاول مرة أخرى
rm -rf build
mkdir build && cd build
cmake ..
make
```

## 🤝 المساهمة

نرحب بالمساهمات! يرجى:

1. Fork المستودع
2. أنشئ فرعًا للميزة (`git checkout -b feature/AmazingFeature`)
3. Commit التغييرات (`git commit -m 'Add some AmazingFeature'`)
4. Push إلى الفرع (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## 📄 الترخيص

هذا المشروع مرخص تحت رخصة MIT - انظر ملف [LICENSE](LICENSE) للتفاصيل.

## 📞 الدعم

للمساعدة والدعم، يرجى:

- فتح issue في GitHub
- التواصل عبر البريد الإلكتروني: support@rafeeq-hajj.com
- زيارة الموقع: https://rafeeq-hajj.com

---

**منصة رفيق الحاج والمعتمر** 🕌

تم الإنشاء بحب وتفاني لخدمة حجاج بيت الله الحرام والمعتمرين
