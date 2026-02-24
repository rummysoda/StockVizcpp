#include "window.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Font.hpp>
#include "imgui.h"
#include "imgui-SFML.h"
#include "implot.h"
#include "stocksgui.h"
#include "TextButton.h"


void runWindow() {   //style none gets rid of titlebar sf::Style::None
    sf::RenderWindow window(sf::VideoMode(1920, 1000), "Stock Visualizer");
    ImGui::SFML::Init(window);
    ImPlot::CreateContext();

    window.setFramerateLimit(60);
    sf::Font font;
    font.loadFromFile("pt-root-ui_vf.ttf");

    std::vector<TextButton*> buttonVec;
    float keyTimeMax = 1000.f;
    float keyTime = keyTimeMax;




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
                    for (size_t i = 0; i< buttonVec.size(); i++) {buttonVec[i]->typedOn(event);}
                    break;
            }

        }


        ImGui::SFML::Update(window,deltaClock.restart());

        // imgui test
        /* ImGui::Begin("Test Panel");
        ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
        ImGui::End();




        // implot test
        /* ImGui::Begin("Plot Test");
        if (ImPlot::BeginPlot("Sample Plot")) {
            static float xs[] = {0,1,2,3,4};
            static float ys[] = {0,1,4,9,16};
            ImPlot::PlotLine("y = x^2", xs, ys, 5);
            ImPlot::EndPlot();
        }
        ImGui::End(); */

        ImGui::Begin("checkbox");
        static bool toggle = false;
        ImGui::Checkbox("toggle", &toggle);
        ImGui::End();

        Demo_CustomPlottersAndTooltips();



        // button
        sf::Vector2f mousePosView = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::B) && keyTime >= keyTimeMax)
        {
            float xx;
            float yy;
            xx = 1500 + buttonVec.size();
            yy = 150 + buttonVec.size() * 100;
            buttonVec.push_back(new TextButton(xx,yy));
        }

        for (size_t i = 0; i < buttonVec.size(); i++) {
            buttonVec[i]->update(window);
            if (buttonVec[i]->isPressed()) {
                for (size_t j = 0; j < buttonVec.size(); j++) {
                    if (j != i) {
                        buttonVec[j]->setSelected(false);
                    }
                }
            }
        }

        if (keyTime < keyTimeMax)
        {
            keyTime += 1.f;
        }
        window.clear(sf::Color::Black);
        for (size_t i = 0; i < buttonVec.size(); i++)
        {
            buttonVec[i]->render(window);
        }
        //render

        ImGui::SFML::Render(window);
        //window.draw(myt);
        window.display();

    }
    for (size_t i = 0; i < buttonVec.size(); i++)
    {
        delete buttonVec[i];
    }
}
