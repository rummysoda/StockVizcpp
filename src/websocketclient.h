#ifndef WEBSOCKETCLIENT_H
#define WEBSOCKETCLIENT_H

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/websocket/ssl.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio/strand.hpp>
#include <nlohmann/json.hpp>
#include <string>
#include <functional>
#include <thread>
#include <mutex>
#include "Stock.h"
#include <unordered_map>

namespace beast = boost::beast;
namespace websocket = beast::websocket;
namespace net = boost::asio;
namespace ssl = net::ssl;
using tcp = net::ip::tcp;

std::string readApiKeyFromFile(const std::string& filename);

class FinnhubWS {
private:
    std::string apiKey_;
    net::io_context ioc_;
    ssl::context ctx_{ssl::context::sslv23_client};
    std::unique_ptr<websocket::stream<ssl::stream<tcp::socket>>> ws_;
    std::mutex wsMutex_;
    std::unordered_map<std::string, Stock*> stocks_;



public:
    FinnhubWS(const std::string& apiKey);
    ~FinnhubWS();
    void connect();
    void subscribe(const std::string& topic, Stock* stock);
    void readLoop();
    void parseMessage(const std::string& message);
    void unsubscribe(const std::string& symbol);
};

#endif //WEBSOCKETCLIENT_H