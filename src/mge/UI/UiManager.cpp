#include <string>
#include <iostream>
#include "mge/UI/UiManager.h"
#include "mge/UI/Menu.h"
#include "mge/config.hpp"
#include <GL/glew.h>
#include <lua.hpp>

//static variables
std::vector<Menu*> UiManager::_menus = std::vector<Menu*>();

UiManager::UiManager(sf::RenderWindow* pWindow) {
	_window = pWindow;

	//init static variables
	_menus.clear();

	_initMenus();

	if(_menus.size() > 0) _activeMenu = _menus[0]; //first menu, if there is one
}

UiManager::~UiManager() {
	//destructor
}

void UiManager::draw() {
	//uncomment this, if we disable the debug hud later
	/**
	//glDisable( GL_CULL_FACE );
	glActiveTexture(GL_TEXTURE0);
	_window->pushGLStates();
	/**/
	
	_drawAll();

	_window->popGLStates();
}

int UiManager::_createMenu(lua_State * state) {
	std::string menuName = lua_tostring(state, 1);
	int imageCount = (int)lua_tointeger(state, 2); //1 images = 1 texture
	int buttonCount = (int)lua_tointeger(state, 3); //1 button = 2 textures

	lua_settop(state, 0); //clear the stack

	Menu* newMenu = new Menu(menuName);
	newMenu->InitMenu(imageCount, buttonCount);
	_menus.push_back(newMenu);

	return 0;
}

int UiManager::_setMenuBackground(lua_State * state) {
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

int UiManager::_addButton(lua_State * state) {
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

		//Create a button here

		break; //menu found, no need to loop further
	}

	return 0;
}

int UiManager::_addImage(lua_State * state) {
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

void UiManager::_drawAll() {
	if(_activeMenu == nullptr) return;

	_window->draw(_activeMenu->GetBackgroundSprite()); //draw the background first
	
	//draw all images second
	for(int i = 0; i < _activeMenu->GetImgCount(); i++) {
		_window->draw(_activeMenu->GetImgAt(i));
	}

	//draw all buttons third

	//draw all texts fourth
}

void UiManager::_initMenus() {
	lua_State* state = luaL_newstate();
	luaL_openlibs(state); // get all libs in state (math, os, io)

	//register functions as global lua functions
	lua_register(state, "CreateMenu", _createMenu);
	lua_register(state, "SetBackground", _setMenuBackground);
	lua_register(state, "AddButton", _addButton);
	lua_register(state, "AddImage", _addImage);

	std::string filename = config::MGE_LUA_PATH + "main.lua";
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
