#include <string>
#include <iostream>
#include <sstream>
#include <iterator>
#include "mge/UI/UiContainer.h"
#include "mge/UI/Menu.h"
#include "mge/managers/SceneManager.h"
#include "mge/managers/InputManager.h"
#include "mge/audio/AudioContainer.h"
#include "mge/config.hpp"
#include <GL/glew.h>
#include <lua.hpp>

//static variables
sf::RenderWindow* UiContainer::_window;
Menu* UiContainer::_activeMenu;
std::vector<Menu*> UiContainer::_menus = std::vector<Menu*>();
std::map<std::string, sf::Font*> UiContainer::_fonts = std::map<std::string, sf::Font*>();

UiContainer::UiContainer(sf::RenderWindow* pWindow) {
	_window = pWindow;

	//init static variables
	_menus.clear();

	//create all menus
	_initMenus();

	//register this as a listener for key events
	SystemEventDispatcher::AddListener(this, "UI");

	SelectMenu("LOADING"); //inital screen when starting the game
}

UiContainer::~UiContainer() {
	//destructor

	for(unsigned i = 0; i < _menus.size(); i++) {
		delete _menus[i];
	}
}

void UiContainer::draw() {
	_drawAll();

	_window->popGLStates();
}

void UiContainer::CloseApp() {
	//close game/window
	_window->close();
}

void UiContainer::SelectMenu(std::string target) {
	std::cout << "Selected menu " + target << std::endl;

	InputManager::SetGameInput(false);
	InputManager::SetMenuInput(true);

	if(target == "HUD") {
		//disable menu input and enable game input
		InputManager::SetGameInput(true);
		InputManager::SetMenuInput(false);

		AudioContainer::StopSound("MAIN_BGM");
		AudioContainer::PlaySound("BGM_LEVEL");
	} else if (target == "MAIN") {
		AudioContainer::StopSound("BGM_LEVEL");
		AudioContainer::PlaySound("MAIN_BGM");
	} else if(target == "PAUSE") {
		AudioContainer::PlaySound("OPEN_PAUSE"); //no bgm
	} else if(target.substr(0, 5) == "LEVEL") {
		AudioContainer::PlaySound("OPEN_RESOLUTION");
	}

	for(unsigned i = 0; i < _menus.size(); i++) {
		if(_menus[i]->GetMenuName() == target) {
			if(_activeMenu != nullptr) _activeMenu->SetActive(false);

			_activeMenu = _menus[i];
			_activeMenu->SetActiveButton(0);

			_activeMenu->SetActive(true);
			return;
		}
	}
}

sf::Font* UiContainer::GetFontByName(std::string fontname) {
	return _fonts[fontname];
}

void UiContainer::drawLoading(int percentage) {
	if(_window->isOpen()) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);

		_window->pushGLStates();

		//modify our percentage text in the loading screen
		for(int i = 0; i < _menus.size(); i++) {
			if(_menus[i]->GetMenuName() == "LOADING") {
				_menus[i]->SetTextAt(0, std::to_string(percentage) + "%"); //hardcoded for at least one text in the loading menu

				break; //no need to loop further since we found our object
			}
		}

		draw();

		_window->display();
	}
}

int UiContainer::_createMenu(lua_State * state) {
	std::string menuName = lua_tostring(state, 1);
	int imageCount = (int)lua_tointeger(state, 2); //1 images = 1 texture
	int buttonCount = (int)lua_tointeger(state, 3); //1 button = 2 textures
	int textCount = (int)lua_tointeger(state, 4);
	bool horizontalNav = lua_toboolean(state, 5);

	lua_settop(state, 0); //clear the stack

	if(menuName == "LOADING") {
		textCount = 1;
	}

	Menu* newMenu = new Menu(menuName);
	newMenu->InitMenu(imageCount, buttonCount, textCount, horizontalNav);
	_menus.push_back(newMenu);

	return 0;
}

int UiContainer::_createFont(lua_State * state) {
	std::string fontName = lua_tostring(state, 1);
	std::string fontFilename = lua_tostring(state, 2);

	lua_settop(state, 0); //clear the stack

	sf::Font* newFont = new sf::Font();
	newFont->loadFromFile(config::MGE_FONT_PATH + fontFilename);

	_fonts[fontName] = newFont; //insert the new font with its identifier into the map

	return 0;
}

int UiContainer::_setBackground(lua_State * state) {
	std::string parentMenu = lua_tostring(state, 1);
	std::string filename = lua_tostring(state, 2);
	lua_settop(state, 0); //clear the stack

	for(unsigned i = 0; i < _menus.size(); i++) {
		if(_menus[i]->GetMenuName() != parentMenu) continue;

		Menu* myMenu = _menus[i];
		myMenu->SetBackground(filename);

		break; //menu found, no need to loop further
	}

	return 0;
}

int UiContainer::_addButton(lua_State * state) {
	std::string parentMenu = lua_tostring(state, 1);
	std::string target = lua_tostring(state, 2);
	std::string activeButtonFile = lua_tostring(state, 3);
	std::string inactiveButtonFile = lua_tostring(state, 4);
	int xPos = (int)lua_tointeger(state, 5);
	int yPos = (int)lua_tointeger(state, 6);
	lua_settop(state, 0); //clear the stack

	for(unsigned i = 0; i < _menus.size(); i++) {
		if(_menus[i]->GetMenuName() != parentMenu) continue;

		Menu* myMenu = _menus[i];
		myMenu->AddButton(activeButtonFile, inactiveButtonFile, xPos, yPos, target);

		break; //menu found, no need to loop further
	}

	return 0;
}

int UiContainer::_addImage(lua_State * state) {
	std::string parentMenu = lua_tostring(state, 1);
	std::string filename = lua_tostring(state, 2);
	int xPos = (int)lua_tointeger(state, 3);
	int yPos = (int)lua_tointeger(state, 4);
	lua_settop(state, 0); //clear the stack

	for(unsigned i = 0; i < _menus.size(); i++) {
		if(_menus[i]->GetMenuName() != parentMenu) continue;

		Menu* myMenu = _menus[i];
		myMenu->AddImage(filename, xPos, yPos);

		break; //menu found, no need to loop further
	}

	return 0;
}

int UiContainer::_addText(lua_State * state) {
	std::string parentMenu = lua_tostring(state, 1);
	std::string usedFont = lua_tostring(state, 2);
	std::string text = lua_tostring(state, 3); //includes potentially special keywords
	int textSize = (int)lua_tointeger(state, 4);
	int textColorR = (int)lua_tointeger(state, 5);
	int textColorG = (int)lua_tointeger(state, 6);
	int textColorB = (int)lua_tointeger(state, 7);
	bool isBold = lua_toboolean(state, 8);
	int xPos = (int)lua_tointeger(state, 9);
	int yPos = (int)lua_tointeger(state, 10);

	lua_settop(state, 0); //clear the stack

	for(unsigned i = 0; i < _menus.size(); i++) {
		if(_menus[i]->GetMenuName() != parentMenu) continue;

		Menu* myMenu = _menus[i];
		myMenu->AddText(usedFont, text, textSize, textColorR, textColorG, textColorB, isBold, xPos, yPos);

		break; //menu found, no need to loop further
	}

	return 0;
}

void UiContainer::_drawAll() {
	if(_activeMenu == nullptr) return;

	if(_activeMenu->GetBackgroundSprite().getTexture() != nullptr) _window->draw(_activeMenu->GetBackgroundSprite()); //draw the background first
	
	//draw all images 
	for(int i = 0; i < _activeMenu->GetImgCount(); i++) {
		_window->draw(_activeMenu->GetImgAt(i));
	}

	//draw all buttons 
	for(int i = 0; i < _activeMenu->GetButtonCount(); i++) {
		_window->draw(_activeMenu->GetButtonAt(i));
	}

	//draw all texts 
	for(int i = 0; i < _activeMenu->GetTextCount(); i++) {
		_window->draw(_activeMenu->GetTextAt(i));
	}
}

void UiContainer::_initMenus() {
	lua_State* state = luaL_newstate();
	luaL_openlibs(state); // get all libs in state (math, os, io)

	//register functions as global lua functions
	lua_register(state, "CreateMenu", _createMenu);
	lua_register(state, "CreateFont", _createFont);
	lua_register(state, "SetBackground", _setBackground);
	lua_register(state, "AddButton", _addButton);
	lua_register(state, "AddImage", _addImage);
	lua_register(state, "AddText", _addText);

	std::string filename = config::MGE_LUA_PATH + "ui.lua";
	luaL_dofile(state, filename.c_str()); //execute lua file

	//calling Menu.init() in lua
	lua_getglobal(state, "Menu"); // Menu table on stack
	if(lua_istable(state, -1)) {
		lua_getfield(state, -1, "init"); //Menu.init on stack
		if(lua_isfunction(state, -1)) {
			if(lua_pcall(state, 0, 0, 0) != 0) { // call Menu.init() from lua, pcall(state, argument amt, result amt, ?)
				printf("Error : %s", lua_tostring(state, -1));
				getchar();
				exit(-3);
			}
		}
	}

	lua_settop(state, 0); //clear the stack
}

void UiContainer::onNotify(sf::Event pEvent) {
	if(pEvent.type != sf::Event::KeyPressed) return;

	//if we pressed esc and we are in the level (for now no hud active), we enter the pause menu
	if(pEvent.key.code == sf::Keyboard::Escape) {
		//Pause or resume the game
		if(_activeMenu->GetMenuName() == "HUD") {
			SelectMenu("PAUSE");
		} else if(_activeMenu->GetMenuName() == "PAUSE") {
			SelectMenu("HUD");
		}	
	}
}