#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>
#include <filesystem>

#include "window.h"
#include "websocketclient.h"
#include "apiclient.h"

int main() {
    try {
        // Start WebSocket client
        std::string apikey = readApiKeyFromFile("apikey.txt");
        FinnhubWS client(apikey);

        client.connect();
        std::this_thread::sleep_for(std::chrono::seconds(2));

        client.subscribe("AAPL");

        std::cout << "WebSocket connected, checking stocks..." << std::endl;
        client.startReading();

        // Run the window
        runWindow();

        std::cout << "Window closed, exiting..." << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;

}