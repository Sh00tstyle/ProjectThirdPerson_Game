#include <iostream>
#include <vector>
#include "mge\UI\Menu.h"
#include "mge\config.hpp"

Menu::Menu(std::string pMenuName) {
	_menuName = pMenuName;
}

Menu::~Menu() {
}

void Menu::InitMenu(int imageCount, int buttonCount) {
	for(int i = 0; i < imageCount; i++) {
		sf::Texture texture;
		sf::Sprite sprite;

		_imageTextures.push_back(texture);
		_imageSprites.push_back(sprite);
	}

	for(int i = 0; i < buttonCount * 2; i++) {
		sf::Texture texture;
		sf::Sprite sprite;

		_buttonTextures.push_back(texture);
		_buttonSprites.push_back(sprite);
	}
}

void Menu::SetBackground(std::string filename) {
	if(_backgroundTexture.loadFromFile(config::MGE_UI_PATH + filename)) {
		_backgroundTexture.setSmooth(true);
		_backgroundSprite.setTexture(_backgroundTexture);
	}
}

sf::Sprite Menu::GetBackgroundSprite() {
	return _backgroundSprite;
}

int Menu::GetImgCount() {
	return _imageSprites.size();
}

sf::Sprite Menu::GetImgAt(int index) {
	return _imageSprites[index];
}

void Menu::AddButton(std::string filename) {
	
}

void Menu::AddImage(std::string filename, int xPos, int yPos) {
	for(unsigned i = 0; i < _imageSprites.size(); i++) {
		//Get the first unsed sprite in the vector
		if(_imageSprites[i].getTexture() == nullptr) {
			if(_imageTextures[i].loadFromFile(config::MGE_UI_PATH + filename)) {
				_imageTextures[i].setSmooth(true);

				_imageSprites[i].setTexture(_imageTextures[i]);
				_imageSprites[i].setPosition(sf::Vector2f((float)xPos, (float)yPos));
			}

			//found an empty sprite so dont do anything else
			break;
		}
	}
}

std::string Menu::GetMenuName() {
	return _menuName;
}
