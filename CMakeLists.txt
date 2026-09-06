cmake_minimum_required(VERSION 3.15)
project(rafeeq-hajj-platform VERSION 1.0.0 LANGUAGES CXX)

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O2")

# Find required packages
find_package(PostgreSQL REQUIRED)
find_package(nlohmann_json 3.2.0 QUIET)
find_package(OpenSSL REQUIRED)
find_package(Threads REQUIRED)

# Add source files
set(SOURCES
    src/main.cpp
    src/core/config/config.cpp
    src/core/database/database_connection.cpp
    src/core/database/connection_pool.cpp
    src/services/gps_service/gps_service.cpp
    src/services/location_service/location_service.cpp
    src/services/trip_service/trip_service.cpp
    src/services/notification_service/notification_service.cpp
    src/services/geofence_service/geofence_service.cpp
    src/api/server.cpp
)

# Add headers
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${PostgreSQL_INCLUDE_DIRS}
)

# Create executable
add_executable(rafeeq_server ${SOURCES})

# Link libraries
target_link_libraries(rafeeq_server
    ${PostgreSQL_LIBRARIES}
    OpenSSL::SSL
    OpenSSL::Crypto
    Threads::Threads
    pthread
)

# Enable testing
enable_testing()
add_subdirectory(tests)
