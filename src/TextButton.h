#ifndef TEXTBUTTON_H
#define TEXTBUTTON_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>

#define DELETE_KEY 8
#define ENTER_KEY 13
#define ESCAPE_KEY 27


class TextButton {
private:
sf::Sprite shape;
sf::Texture Idle;
sf::Texture Hover;
sf::Texture Presssed;
sf::Texture Disabled;
int buttonState;
sf::Font font;


sf::Text textb;
std::ostringstream text;
bool isSelected = false;
int limit = 5;

//text functions
void inputLogic(int charTyped) {
        if (charTyped != ENTER_KEY && charTyped != ESCAPE_KEY && charTyped != DELETE_KEY) {
            this->text << static_cast<char>(charTyped);
        }
        else if(charTyped == DELETE_KEY) {
            if(this->text.str().length() > 0) {
                deletelastChar();
            }
        }
        this->textb.setString(this->text.str() + "|");
    }

    void deletelastChar() {
        std::string t = this->text.str();
        std::string newT = "";
        for(int i = 0; i < t.length() - 1; i++) {
            newT += t[i];
        }
        this->text.str("");
        this->text << newT;

        this->textb.setString(this->text.str());
    }

public:
	TextButton(float x, float y) {
		this->font.loadFromFile("pt-root-ui_vf.ttf");
		this->Idle.loadFromFile("../interface/TB/buttonDefault.png");
		this->Hover.loadFromFile("../interface/TB/buttonHovered.png");
		this->Presssed.loadFromFile("../interface/TB/buttonPressed.png");
		this->Disabled.loadFromFile("../interface/TB/buttonDisabled.png");
		this->buttonState = IDLE;

		this->shape.setTexture(this->Idle);
		this->shape.setPosition({x,y});

		this->textb.setPosition({x,y+5});
		this->textb.setFont(this->font);
		this->textb.setCharacterSize(20);

		 if(isSelected) {
            this->textb.setString("|");
        }
        else {
            this->textb.setString("");
        }
	}


	// text settings
	void setSelected(bool sel) {
        isSelected = sel;
        if(!sel) {
            std::string t = this->text.str();
            std::string newT = " ";
            for(int i = 0; i < t.length() ; i++) {
                newT += t[i];
            }
            this->textb.setString(newT);
        }
		else {
			this->textb.setString(this->text.str() + "|");
		}
    }

    std::string getText() {
        return this->text.str();
    }

	void clearText() {
		this->text.str("");
		this->textb.setString("");
	}

    void drawTo(sf::RenderWindow &window) {
        window.draw(this->textb);
    }


    void typedOn(sf::Event &input) {
        if(isSelected) {
            int charTyped = input.text.unicode;
            if(charTyped < 128) {

                if(this->text.str().length() <= limit) {
                    inputLogic(charTyped);
                }
                else if (charTyped == DELETE_KEY) {
                	deletelastChar();
                }
            }
        }
    }

    // button settings
	bool isPressed() const {return this->buttonState == PRESSED;}
	bool isHover() const {return this->buttonState == HOVER;}
	bool isDisabled() const {return this->buttonState == DISABLED;}

	void setButtonState(BUTTON_STATE state) {
		this->buttonState = state;
		switch(state) {
			case IDLE:     this->shape.setTexture(this->Idle);     break;
			case HOVER:    this->shape.setTexture(this->Hover);    break;
			case PRESSED:  this->shape.setTexture(this->Presssed); break;
			case DISABLED: this->shape.setTexture(this->Disabled); break;
		}
	}
	void update(const sf::RenderWindow& window) {
		if (isDisabled()) return;
		sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
		if(this->shape.getGlobalBounds().contains(mousePos)) {
			this->shape.setTexture(this->Hover);
			this->buttonState = HOVER;
			this->textb.setFillColor(sf::Color::White);
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				//selected
				this->shape.setTexture(this->Presssed);
				this->buttonState = PRESSED;
				this->textb.setFillColor(sf::Color(56,23,216));
				this->setSelected(true);
			}
		}
		else {
			this->shape.setTexture(this->Idle);
			this->buttonState = IDLE;
			this->textb.setFillColor(sf::Color(90,63,225));
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
				this->setSelected(false);
			}
			if (!this->shape.getGlobalBounds().contains(mousePos) && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {this->setSelected(false);}
		}
		// make sure in the while loop to take even for textenetered to set textbox[i]->textb,typedOn(event);
	}


	void render(sf::RenderTarget& target){
		target.draw(this->shape);
		target.draw(this->textb);

	}

};
#endif //TEXTBUTTON_H
