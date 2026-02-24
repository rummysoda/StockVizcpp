#ifndef ADDBUTTON_H
#define ADDBUTTON_H

#include <SFML/Graphics.hpp>

#define DELETE_KEY 8
#define ENTER_KEY 13
#define ESCAPE_KEY 27

enum BUTTON_STATE{IDLE = 0, HOVER = 1, PRESSED = 2, DISABLED = 3};

class AddButton {
private:
    sf::Sprite shape;
    sf::Texture Idle;
    sf::Texture Hover;
    sf::Texture Presssed;
    sf::Texture Disabled;
    int buttonState;

public:
    AddButton(float x, float y) {
        this->Idle.loadFromFile("plusMain.png");
        this->Hover.loadFromFile("plusHover.png");
        this->Presssed.loadFromFile("plusClicked.png");
        this->Disabled.loadFromFile("plusDisabled.png");
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
#endif ADDBUTTON_H