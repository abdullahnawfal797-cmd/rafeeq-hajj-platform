#include <iostream>
#include "core/config/config.h"
#include "api/server.h"

int main(int argc, char* argv[]) {
    try {
        std::cout << "🕌 منصة رفيق الحاج والمعتمر" << std::endl;
        std::cout << "Starting Rafeeq Hajj Platform..." << std::endl;

        // Load configuration
        Config config;
        config.loadFromFile("config/config.json");
        
        std::cout << "✓ Configuration loaded" << std::endl;

        // Initialize server
        APIServer server(config.getServerPort());
        
        std::cout << "✓ Server initialized on port " << config.getServerPort() << std::endl;
        
        // Start server
        server.start();
        
        std::cout << "✓ Server started successfully" << std::endl;
        
        // Keep server running
        server.run();
        
    } catch (const std::exception& e) {
        std::cerr << "❌ Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
