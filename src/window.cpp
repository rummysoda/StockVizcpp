#include "window.h"
#include <thread>
#include <SFML/Window.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Font.hpp>
#include "websocketclient.h"
#include "AddButton.h"
#include "apiclient.h"
#include "CloseButton.h"
#include "imgui.h"
#include "imgui-SFML.h"
#include "implot.h"
#include "Stock.h"
#include "stocksgui.h"
#include "TextButton.h"
#include <unordered_set>
#include "imGuiColor.h"

void runWindow(FinnhubWS& client) {   //style none gets rid of titlebar sf::Style::None
    sf::RenderWindow window(sf::VideoMode(1920, 1000), "Stock Visualizer");
    window.requestFocus();
    ImGui::SFML::Init(window);

    ImVec4 maincolor = ImVec4(188,176,245,1);
    SetupImGuiStyle(maincolor);

    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->Clear();
    ImFont* font1 = io.Fonts->AddFontFromFileTTF("../Interface/font/AcuminR.ttf", 13);
    ImGui::SFML::UpdateFontTexture();
    ImPlot::CreateContext();


    std::string APIKEY = readApiKeyFromFile("apikey.txt");
    window.setFramerateLimit(60);
    sf::Font font;
    font.loadFromFile("pt-root-ui_vf.ttf");

    float xx = 1500;
    float yy = 150;
    std::vector<StockEntry*> entries;
    std::unordered_set<std::string> symbolNames;

    StockEntry* first = new StockEntry();
    first->textButton = new TextButton(xx,yy);
    first->addButton = new AddButton(xx + 150,yy);
    first->closeButton = nullptr;
    entries.push_back(first);

    sf::Texture existsTexture;
    existsTexture.loadFromFile("../Interface/WarningB/exists.png");
    sf::Sprite errorExists(existsTexture);
    errorExists.setScale(0.4f,0.4f);

    sf::Texture Background;
    Background.loadFromFile("../Interface/BG/bg2.png");
    sf::Sprite bg(Background);
    sf::Texture errorTexture;
    errorTexture.loadFromFile("../interface/WarningB/NotFound.png");
    sf::Sprite errorSprite(errorTexture);
    errorSprite.setScale(0.4f, 0.4f);

    sf::Clock deltaClock;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(window, event);
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::TextEntered:
                    for (auto& entry : entries) {entry->textButton->typedOn(event);}
                    break;
            }
        }
        ImGui::SFML::Update(window,deltaClock.restart());

        for(size_t i = 0; i < entries.size(); i++) {
            auto& entry = *entries[i];

            entry.textButton->update(window);
            entry.addButton->update(window);
            if(entry.closeButton != nullptr) {entry.closeButton->update(window);}

            if(entry.addButton->isPressed() && !entry.isLoading && !entry.textButton->getText().empty()) {

                if(entry.isActive) {
                    if(i>0 && entries[i-1]->closeButton != nullptr)
                        entries[i-1]->closeButton->setButtonState(DISABLED);
                    float newY = yy + entries.size() * 50;
                    StockEntry* newEntry = new StockEntry();
                    newEntry->textButton  = new TextButton(xx, newY);
                    newEntry->addButton   = new AddButton(xx + 150, newY);
                    newEntry->closeButton = new CloseButton(xx + 200, newY);
                    entries.push_back(newEntry);
                    entry.addButton->setButtonState(DISABLED);
                    break;
                }

                std::string symbol = entry.textButton->getText();
                if(!validateSymbol(symbol,APIKEY)) {
                    entry.isInvalid = true;
                    entry.errorClock.restart();
                    entry.isLoading = false;
                    break;
                }
                if(symbolNames.find(symbol) != symbolNames.end()) {
                    entry.exists = true;
                    entry.errorClock.restart();
                    entry.isLoading = false;
                    break;
                } else {
                    symbolNames.insert(symbol);
                }

                entry.stock.symbol = symbol;
                entry.isLoading = true;

                entry.closeButton = new CloseButton(xx + 200,yy + i * 50);

                // disables previous entry
                entry.textButton->setButtonState(DISABLED);
                entry.addButton->setButtonState(DISABLED);
                entry.closeButton->setButtonState(DISABLED);

                //disable closing previous entry
                if(i>0 && entries[i-1]->closeButton != nullptr) {entries[i-1]->closeButton->setButtonState(DISABLED);}

                // add new entry
                float newY = yy + entries.size() * 50;
                StockEntry* newEntry = new StockEntry();
                newEntry->textButton = new TextButton(xx,newY);
                newEntry->addButton = new AddButton(xx+150,newY);
                newEntry->closeButton = new CloseButton(xx + 200,newY);
                entries.push_back(newEntry);

                entry.loadThread = std::thread([&entry, symbol, &client]() {
                    loadStock(symbol, entry.stock);
                    client.subscribe(symbol, &entry.stock);
                    entry.isLoading = false;
                    entry.isActive  = true;
                });
                entry.loadThread.detach();
                break;
            }

            if(entry.closeButton != nullptr && entry.closeButton->isPressed()) {
                if(i == 0) {
                    client.unsubscribe(entry.stock.symbol);
                    symbolNames.erase(entry.stock.symbol);
                    entry.stock.clear();
                    entry.textButton->clearText();
                    entry.textButton->setButtonState(IDLE);
                    entry.addButton->setButtonState(IDLE);
                    delete entry.closeButton;
                    entry.closeButton = nullptr;
                    entry.isActive = false;
                    entry.isLoading = false;
                    if(entries.size() > 1 && !entries.back()->isActive && !entries.back()->isLoading) {
                        delete entries.back();
                        entries.pop_back();
                    }
                    if(entries.size() > 1) {
                        entries.back()->addButton->setButtonState(IDLE);
                    }
                } else {
                    client.unsubscribe(entry.stock.symbol);
                    symbolNames.erase(entry.stock.symbol);
                    // re enable previous row
                    if( i> 0 && entries[i-1]->closeButton != nullptr) {
                        entries[i-1]->closeButton->setButtonState(IDLE);
                        entries[i-1]->addButton->setButtonState(IDLE);
                    }
                    delete entries[i];
                    entries.erase(entries.begin() + i); // make sure to remove the pointer from the vector
                }
                break;
            }
        }
        if(entries.size() > 0) {
            Demo_CustomPlottersAndTooltips(entries);
        }


        //render
        window.clear(sf::Color(20,20,20));
        //window.draw(bg);
        for(auto& entry : entries) {
            entry->textButton->render(window);
            entry->addButton->render(window);
            if(entry->closeButton != nullptr) {entry->closeButton->render(window);}
        }
        for(auto& entry : entries) {
            if(entry->isInvalid) {
                if(entry->errorClock.getElapsedTime().asSeconds() < 2.f) {
                    errorSprite.setPosition(20, window.getSize().y - errorTexture.getSize().y - 20);
                    window.draw(errorSprite);
                } else {entry->isInvalid = false;}
            }
            if(entry->exists) {
                if(entry->errorClock.getElapsedTime().asSeconds() < 2.f) {
                    errorExists.setPosition(20, window.getSize().y - errorTexture.getSize().y - 20);
                    window.draw(errorExists);
                } else {entry->exists = false;}
            }
        }

        ImGui::SFML::Render(window);
        window.display();
    }
    for (auto& entry : entries) {delete entry;}
    ImPlot::DestroyContext();
    ImGui::SFML::Shutdown();
}