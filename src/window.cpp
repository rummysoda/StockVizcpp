#include "window.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Font.hpp>
#include "imgui.h"
#include "imgui-SFML.h"
#include "implot.h"

void runWindow()
{
    sf::RenderWindow window(sf::VideoMode(1920, 1000), "Stock Visualizer");
    ImGui::SFML::Init(window);
    ImPlot::CreateContext();

    window.setFramerateLimit(60);

    sf::Font font;
    font.loadFromFile("pt-root-ui_vf.ttf");

    sf::Text myt;
    myt.setFont(font);
    myt.setString("Hello World!");
    myt.setCharacterSize(40);
    myt.setPosition(300,400);
    myt.setFillColor(sf::Color::Green);

    sf::Clock deltaClock;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(window, event);
            if (event.type == sf::Event::Closed) {window.close();}
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {window.close();}
        }

        ImGui::SFML::Update(window,deltaClock.restart());

        // imgui test
        ImGui::Begin("Test Panel");
        ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
        ImGui::End();

        // implot test
        ImGui::Begin("Plot Test");
        if (ImPlot::BeginPlot("Sample Plot")) {
            static float xs[] = {0,1,2,3,4};
            static float ys[] = {0,1,4,9,16};
            ImPlot::PlotLine("y = x^2", xs, ys, 5);
            ImPlot::EndPlot();
        }
        ImGui::End();

        //render

        window.clear(sf::Color::Black);
        ImGui::SFML::Render(window);
        window.draw(myt);


        window.display();
    }
}