#include <string>
#include <fstream>
#include <iostream>
#include "rapidcsv.h"
#include "apiclient.h"
#include "Stock.h"
#include <mutex>
#include <nlohmann/json.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio/ip/tcp.hpp>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
namespace ssl = net::ssl;
using tcp = net::ip::tcp;

bool validateSymbol(const std::string &symbol, const std::string &apiKey) {
    try {
        net::io_context ioc;
        ssl::context ctx(ssl::context::sslv23_client);
        tcp::resolver resolver(ioc);
        auto results = resolver.resolve("finnhub.io", "443");

        ssl::stream<tcp::socket> stream(ioc, ctx);
        SSL_set_tlsext_host_name(stream.native_handle(), "finnhub.io");

        net::connect(stream.next_layer(), results);
        stream.handshake(ssl::stream_base::client);

        std::string target = "/api/v1/search?q=" + symbol + "&exchange=US&token=" + apiKey;
        http::request<http::string_body> req(http::verb::get, target, 11);
        req.set(http::field::host, "finnhub.io");
        http::write(stream, req);

        beast::flat_buffer buffer;
        http::response<http::string_body> res;
        http::read(stream, buffer, res);

        auto j = nlohmann::json::parse(res.body());
        if (j["count"].get<int>() == 0) return false;

        //check for first result
        std::string firstSymbol = j["result"][0]["symbol"].get<std::string>();
        std::cout << "First match: " << firstSymbol << std::endl;
        return firstSymbol == symbol;
    }catch (std::exception& e) {
        std::cout << "Validate error: " << e.what() << std::endl;
        return false;
    }
}


static std::mutex mtx; // static so its shared across all calls

void writeStock (const std::string& symbol) {
    std::ofstream outfile("../src/stockname.txt");
    outfile << symbol;
    outfile.close();
}

void readStock (const std::string& symbol,Stock& stocks) {
    // 130 rows
    std::string filename = "../src/" + symbol + ".csv";
    rapidcsv::Document doc(filename);
    stocks.dates = doc.GetColumn<double>(0);
    stocks.open = doc.GetColumn<double>(1);
    stocks.high = doc.GetColumn<double>(2);
    stocks.low = doc.GetColumn<double>(3);
    stocks.close = doc.GetColumn<double>(4);
    std::remove(filename.c_str());


    std::cout << "Loaded " << stocks.dates.size() << " rows for " << symbol << std::endl;
    std::cout << "First date: " << stocks.dates[0] << " close: " << stocks.close[0] << std::endl;
    std::cout << "Last date: " << stocks.dates.back() << " close: " << stocks.close.back() << std::endl;
}

void loadStock(const std::string& symbol,Stock& stock ) {

    std::lock_guard<std::mutex> lock(mtx); // making sure the first api call finishes before adding the second stock symbol to the text file.

    writeStock(symbol);
    system("..\\.venv\\Scripts\\python.exe ../src/fetcher.py");
    readStock(symbol,stock);

}

