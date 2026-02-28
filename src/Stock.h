//
// Created by rummy on 27/2/2026.
//

#ifndef STOCK_H
#define STOCK_H

#include <mutex>
#include <vector>
#include <string>

struct Candle {
    double timestamp;
    float open = 0, high = 0, low = 0, close = 0;
    bool isOpen = false;
};

struct Stock {
    std::string symbol;
    std::vector<double> open;
    std::vector<double> high;
    std::vector<double> low;
    std::vector<double> close;
    std::vector<double> dates;
    Candle currentCandle;
    std::mutex mtx;
    int candleIntervalSeconds = 60;
    Stock() {}

    void addTick(float price, double timestamp) {
        std::lock_guard<std::mutex> lock(mtx);
        if(!currentCandle.isOpen) { currentCandle = {timestamp, price, price, price, price, true}; }
        else if (timestamp - currentCandle.timestamp >= candleIntervalSeconds) {
            dates.push_back(currentCandle.timestamp);
            open.push_back(currentCandle.open);
            high.push_back(currentCandle.high);
            low.push_back(currentCandle.low);
            close.push_back(currentCandle.close);
            currentCandle = {timestamp, price, price, price,price, true};
        }
        else {
            currentCandle.high = std::max(currentCandle.high, price);
            currentCandle.low = std::min(currentCandle.low, price);
            currentCandle.close = price;
        }
    }
};




#endif //STOCK_H
