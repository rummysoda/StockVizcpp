#include "websocketclient.h"
#include <iostream>
#include <nlohmann/json.hpp>
#include <boost/asio/strand.hpp>
#include <string>
#include <fstream>
#include <sstream>



std::string readApiKeyFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Failed to open file " << filename << std::endl;
    }

    std::string apiKey;
    std::getline(file, apiKey);

    if (apiKey.empty()) {
        std::cout << "No API key found" << std::endl;
    }

    return apiKey;
}



FinnhubWS::FinnhubWS(const std::string &apiKey) : apiKey_(apiKey) {}

FinnhubWS::~FinnhubWS() {
    ioc_.stop();
}

void FinnhubWS::connect() {
        try {
            tcp::resolver resolver{ioc_};
            auto results = resolver.resolve("ws.finnhub.io","443");

            ws_ = std::make_unique<websocket::stream<ssl::stream<tcp::socket>>>((ioc_), ctx_);

            auto& ssl_stream = ws_->next_layer();
            auto& tcp_socket = ssl_stream.next_layer();

            net::connect(tcp_socket, results);
            ssl_stream.handshake(ssl::stream_base::client);
            ws_->handshake("ws.finnhub.io", "/?token=" + apiKey_);

            std::cout << "connected successfully" << std::endl;
        } catch (std::exception const& e) {
            std::cout << "connection failed: " << e.what() << std::endl;
        }
    }

void FinnhubWS::subscribe(const std::string& symbol,Stock* stock) {
    if (!ws_) return;
    std::lock_guard<std::mutex> lock(wsMutex_);
    stocks_[symbol] = stock;
    std::string message = R"({"type":"subscribe","symbol":")" + symbol + R"("})";
    ws_->write(net::buffer(message));
    std::cout << "Subscribed to " << symbol << std::endl;
}

void FinnhubWS::readLoop() {
    while (true) {
        beast::flat_buffer buffer;
        ws_->read(buffer);

        std::string message = beast::buffers_to_string(buffer.data());
        parseMessage(message);
    }
}

void FinnhubWS::parseMessage(const std::string& message) {
    std::cout << "Received: " << message << std::endl;

    try {
        auto j = nlohmann::json::parse(message);

        if (j.contains("type") && j["type"] == "trade") {
            if (j.contains("data")) {
                auto data = j["data"];

                for (auto& trade : data) {
                    std::string symbol = trade["s"];
                    double price = trade["p"];
                    double timestamp = trade["t"];

                    std::cout << "STOCK: " << symbol << " Price: $" << price << " Time:" << timestamp << std::endl;
                    auto t = stocks_.find(symbol);
                    if (t != stocks_.end()) {
                        t->second->addTick(price,timestamp);
                    }
                }
            }
        }

    } catch (nlohmann::json::exception const& e) {
        std::cout << "Parse error: " << e.what() << std::endl;
    }
}


