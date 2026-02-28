//
// Created by rummy on 10/2/2026.
//

#ifndef APICLIENT_H
#define APICLIENT_H
#include "Stock.h"


bool validateSymbol(const std::string& symbol, const std::string& apiKey);

void writeStock(const std::string& symbol);
void readStock(const std::string& symbol,Stock& stocks);
void loadStock(const std::string& symbol,Stock& stocks);
bool Status();


#endif //APICLIENT_H
