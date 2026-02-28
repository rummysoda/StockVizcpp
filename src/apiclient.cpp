#include <string>
#include <fstream>
#include <iostream>
#include "rapidcsv.h"
#include "apiclient.h"
#include "Stock.h"
#include <mutex>

static std::mutex mtx; // static so its shared across all calls

void writeStock (std::string& symbol) {
    std::ofstream outfile("../src/stockname.txt");
    outfile << symbol;
    outfile.close();
}

void readStock (std::string& symbol,Stock& stocks) {
    // 130 rows
    std::string filename = "../src/" + symbol + ".csv";
    rapidcsv::Document doc(filename);
    stocks.dates = doc.GetColumn<double>(0);
    stocks.open = doc.GetColumn<float>(1);
    stocks.high = doc.GetColumn<float>(2);
    stocks.low = doc.GetColumn<float>(3);
    stocks.close = doc.GetColumn<float>(4);
    std::remove(filename.c_str());
}

void loadStock(std::string& symbol,Stock& stock ) {

    std::lock_guard<std::mutex> lock(mtx); // making sure the first api call finishes before adding the second stock symbol to the text file.

    writeStock(symbol);
    system("python ../src/fetcher.py");
    readStock(symbol,stock);

}

