#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>
#include <filesystem>
#include "apiclient.h"
#include "window.h"
#include "websocketclient.h"
#include "apiclient.h"

int main() {
    try {
        std::string apikey = readApiKeyFromFile("apikey.txt");
        FinnhubWS client(apikey);

        std::thread t([&client]() {
            client.connect();
            client.readLoop();
        });
        t.detach();

        runWindow(client);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}