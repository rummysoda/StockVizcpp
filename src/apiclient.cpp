#include <string>
#include <fstream>
#include "rapidcsv.h"
#include "apiclient.h"

void writeStock (std::string& stock) {
    std::ofstream outfile("src/stockname.txt");
    outfile.open ("src/stockname.txt");
    outfile << stock;
    outfile.close();
}

void readStock (std::string& stock) {
    // 125 rows
    std::string filename = "src/" + stock + ".csv";
    std::ifstream infile(filename);
}

bool Status() {
    std::ifstream infile("src/stockname.txt");
    std::string line;
    std::getline(infile, line);
    if (line == "") {return true;}
    return false;
    infile.close();
}

