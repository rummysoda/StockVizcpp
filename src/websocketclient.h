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

namespace beast = boost::beast;
namespace websocket = beast::websocket;
namespace net = boost::asio;
namespace ssl = net::ssl;
using tcp = net::ip::tcp;

std::string readApiKeyFromFile(const std::string& filename);

class FinnhubWS {
private:
    std::string apiKey_;
    std::function<void(std::string, float)> onPriceUpdate_;
    net::io_context ioc_;
    ssl::context ctx_{ssl::context::sslv23_client};
    std::unique_ptr<websocket::stream<ssl::stream<tcp::socket>>> ws_;
    std::thread ioThread_;
    std::thread readThread_;

    void readLoop();
    void parseMessage(const std::string& message);

public:
    FinnhubWS(const std::string& apiKey);
    ~FinnhubWS();
    void connect();
    void subscribe(const std::string& topic);
    void setOnPriceUpdate(std::function<void(std::string, float)> callback);
    void startReading();
};

#endif //WEBSOCKETCLIENT_H