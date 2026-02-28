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


struct StockEntry {
    TextButton* textButton;
    CloseButton* closeButton;
    AddButton* addButton;
    Stock stock;
    bool isActive  = false;
    bool isLoading = false;
    bool isInvalid = false;
    std::thread loadThread;
    sf::Clock errorClock;
};

void runWindow(FinnhubWS& client) {   //style none gets rid of titlebar sf::Style::None
    sf::RenderWindow window(sf::VideoMode(1920, 1000), "Stock Visualizer");
    ImGui::SFML::Init(window);
    ImPlot::CreateContext();


    std::string APIKEY = readApiKeyFromFile("apikey.txt");
    window.setFramerateLimit(60);
    sf::Font font;
    font.loadFromFile("pt-root-ui_vf.ttf");

    float xx = 1500;
    float yy = 150;
    std::vector<StockEntry*> entries;

    StockEntry* first = new StockEntry();
    first->textButton = new TextButton(xx,yy);
    first->addButton = new AddButton(xx + 150,yy);
    first->closeButton = nullptr;
    entries.push_back(first);

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
                std::string symbol = entry.textButton->getText();
                entry.stock.symbol = symbol;
                entry.isLoading = true;

                if (i>0) {entry.closeButton = new CloseButton(xx + 200,yy + i * 50);}

                // disables previous row
                entry.textButton->setButtonState(DISABLED);
                entry.addButton->setButtonState(DISABLED);
                if(i>0 && entries[i-1]->closeButton != nullptr) {entries[i-1]->closeButton->setButtonState(DISABLED);}

                // add new row
                float newY = yy + entries.size() * 50;
                StockEntry* newEntry = new StockEntry();
                newEntry->textButton = new TextButton(xx,newY);
                newEntry->addButton = new AddButton(xx+150,newY);
                newEntry->closeButton = nullptr;
                entries.push_back(newEntry);


                entry.loadThread = std::thread([&entry, &entries , symbol, &client,&errorSprite,&APIKEY,i]() {
                    if(!validateSymbol(symbol, APIKEY)) {
                        delete entries.back();
                        entries.pop_back();

                        delete entry.closeButton;
                        entry.closeButton = nullptr;

                        entry.textButton->setButtonState(IDLE);
                        entry.addButton->setButtonState(IDLE);

                        if (i > 0 && entries[i-1]->closeButton != nullptr){entries[i-1]->closeButton->setButtonState(IDLE);}

                        entry.isInvalid = true;
                        entry.errorClock.restart();
                        entry.isLoading = false;
                        return;
                    }
                    loadStock(symbol, entry.stock);
                    client.subscribe(symbol, &entry.stock);
                    entry.isLoading = false;
                    entry.isActive = true;
                });

                entry.loadThread.detach();
            }

            if(entry.closeButton != nullptr && entry.closeButton->isPressed()) {
                if(entries.size() == 1) {
                    entry.textButton->clearText();
                    entry.textButton->setButtonState(IDLE);
                    entry.addButton->setButtonState(IDLE);
                    delete entry.closeButton;
                    entry.closeButton = nullptr;
                    entry.isActive = false;
                    entry.isLoading = false;
                } else {
                    delete entries.back();
                    entries.pop_back();

                    // re enable previous row
                    if( i> 0 && entries[i-1]->closeButton != nullptr) { entries[i-1]->closeButton->setButtonState(IDLE); }

                    delete entries[i];
                    entries.erase(entries.begin() + i); // make sure to remove the pointer from the vector
                }
                break;
            }
        }

        Demo_CustomPlottersAndTooltips();



        //render
        window.clear(sf::Color::Black);
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
        }

        ImGui::SFML::Render(window);
        window.display();

    }
    for (auto& entry : entries) {delete entry;}
    ImPlot::DestroyContext();
    ImGui::SFML::Shutdown();
}
