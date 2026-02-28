//
// Created by rummy on 27/2/2026.
//

#ifndef STOCK_H
#define STOCK_H

#include <vector>
#include <string>

struct Stock {
    std::string symbol;
    std::vector<float> open;
    std::vector<float> high;
    std::vector<float> low;
    std::vector<float> close;
    std::vector<double> dates;

    Stock() {}
};



#endif //STOCK_H
