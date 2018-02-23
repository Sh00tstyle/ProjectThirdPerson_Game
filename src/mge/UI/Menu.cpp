#include <iostream>
#include <vector>
#include <sstream>
#include <iterator>
#include <SFML/Window/Keyboard.hpp>
#include "mge/UI/UiManager.h"
#include "mge/UI/Menu.h"
#include "mge/managers/SceneManager.h"
#include "mge/config.hpp"

Menu::Menu(std::string pMenuName) {
	_menuName = pMenuName;

	_activeButton = 0;
	_buttonCount = 0;
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

	for(int i = 0; i < buttonCount; i++) {
		sf::Texture activeTexture;
		sf::Texture inactiveTexture;
		sf::Sprite sprite;

		_buttonActiveTextures.push_back(activeTexture);
		_buttonInactiveTextures.push_back(inactiveTexture);
		_buttonSprites.push_back(sprite);
	}

	_buttonCount = buttonCount;
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

void Menu::AddButton(std::string activeFilename, std::string inactiveFilename, int xPos, int yPos, std::string target) {
	for(unsigned i = 0; i < _buttonSprites.size(); i++) {
		//Get the first unsed sprite in the vector
		if(_buttonSprites[i].getTexture() == nullptr) {
			//initialize buttons
			if(_buttonActiveTextures[i].loadFromFile(config::MGE_UI_PATH + activeFilename)) {
				_buttonActiveTextures[i].setSmooth(true);
			}

			if(_buttonInactiveTextures[i].loadFromFile(config::MGE_UI_PATH + inactiveFilename)) {
				_buttonInactiveTextures[i].setSmooth(true);
			}

			if(i == 0) {
				_buttonSprites[i].setTexture(_buttonActiveTextures[i]);
			} else {
				_buttonSprites[i].setTexture(_buttonInactiveTextures[i]);
			}
			
			_buttonSprites[i].setPosition(sf::Vector2f((float)xPos, (float)yPos));
			_buttonCommands.push_back(target);

			//found an empty sprite so dont do anything else
			return;
		}
	}
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
			return;
		}
	}
}

int Menu::GetButtonCount() {
	return _buttonCount;
}

sf::Sprite Menu::GetButtonAt(int index) {
	return _buttonSprites[index];
}

int Menu::GetActiveButton() {
	return _activeButton;
}

void Menu::SetActiveButton(int index) {
	_activeButton = index;
}

std::string Menu::GetMenuName() {
	return _menuName;
}

bool Menu::IsActive() {
	return _isActive;
}

void Menu::SetActive(bool value) {
	_isActive = value;
}

void Menu::ProcessInput(sf::Event event) {
	//only do it if the event was a keypress and if the menu is active
	if(!_isActive || event.type != sf::Event::KeyPressed || _buttonCount == 0) return;

	if(event.key.code == sf::Keyboard::W) {
		_activeButton--;

		if(_activeButton < 0) _activeButton = _buttonCount - 1;
	}else if(event.key.code == sf::Keyboard::S) {
		_activeButton++;

		if(_activeButton >= _buttonCount) _activeButton = 0;
	} else if(event.key.code == sf::Keyboard::Space) {
		_useButton(_activeButton);
	}

	_updateButtons();
}

void Menu::SetUiManager(UiManager * manager) {
	_manager = manager;
}

void Menu::_useButton(int index) {
	if(_buttonCount <= 0) return;

	//apply functionality of the button

	std::string buttonCmd = _buttonCommands[index];
	std::istringstream iss(buttonCmd);
	std::vector<std::string> results((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>()); //split by " "

	std::string relevantString = results[0]; //should always be filled

	std::cout << relevantString << std::endl;

	if(relevantString == "EXIT") {
		_manager->CloseApp();
	} else if(relevantString == "LEVEL") {
		int levelIndex = std::stoi(results[1]); //results[1] should always contain the level index

		SceneManager::LoadSceneAtIndex(levelIndex - 1);

		_manager->SelectMenu("");
	} else {
		_manager->SelectMenu(buttonCmd);
	}
}

void Menu::_updateButtons() {
	//update image of the button

	for(unsigned i = 0; i < _buttonSprites.size(); i++) {
		if(_activeButton == i) {
			_buttonSprites[i].setTexture(_buttonActiveTextures[i]);
		} else {
			_buttonSprites[i].setTexture(_buttonInactiveTextures[i]);
		}
	}
}