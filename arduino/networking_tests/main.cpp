#include "WebSocketClient.hpp"
#include <iostream>

int main() {
    WebSocketClient client;
    
    std::string uri = "ws://localhost:8080"; // Example WebSocket echo server
    
    if (client.Connect(uri)) {
        client.Start();
        
        std::string message;
        while (true) {
            std::cout << "Enter message (or 'exit' to quit): ";
            std::getline(std::cin, message);
            
            if (message == "exit") {
                break;
            }
            
            client.Send(message);
        }
        
        client.Stop();
        client.Disconnect();
    }

    return 0;
}
