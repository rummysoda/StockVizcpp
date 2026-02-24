//
// Created by rummy on 24/2/2026.
//

#ifndef CLOSEBUTTON_H
#define CLOSEBUTTON_H

#include <SFML/Graphics.hpp>

#define DELETE_KEY 8
#define ENTER_KEY 13
#define ESCAPE_KEY 27


class CloseButton {
private:
    sf::Sprite shape;
    sf::Texture Idle;
    sf::Texture Hover;
    sf::Texture Presssed;
    sf::Texture Disabled;
    int buttonState;

public:
    CloseButton(float x, float y) {
        this->Idle.loadFromFile("closeMain.png");
        this->Hover.loadFromFile("closeHovered.png");
        this->Presssed.loadFromFile("closePressed.png");
        this->Disabled.loadFromFile("closeDisabled.png");
        this->buttonState = IDLE;
        this->shape.setTexture(this->Idle);
        this->shape.setPosition({x, y});
    }

    // button settings
    bool isPressed() const {return this->buttonState == PRESSED;}
    bool isHover() const {return this->buttonState == HOVER;}
    bool isDisabled() const {return this->buttonState == DISABLED;}
    void update(const sf::RenderWindow& window) {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        if(!isDisabled()) {
            if(this->shape.getGlobalBounds().contains(mousePos)) {
                this->shape.setTexture(this->Hover);
                this->buttonState = HOVER;
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    this->shape.setTexture(this->Presssed);
                    this->buttonState = PRESSED;
                }
            } else {
                this->shape.setTexture(this->Idle);
                this->buttonState = IDLE;
            }
        } else {
            this->shape.setTexture(this->Disabled);
        }
    }

    void render(sf::RenderTarget& target){
        target.draw(this->shape);

    }

};

#endif //CLOSEBUTTON_H
