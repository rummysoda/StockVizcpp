#include "window.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Font.hpp>

void runWindow()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Stock Visualizer");
    window.setFramerateLimit(60);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);


        window.display();
    }
}