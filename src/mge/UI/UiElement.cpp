#include <string>
#include <iostream>
#include "mge/UI/UiElement.h"
#include "mge/config.hpp"


#include <lua.hpp>

UiElement::UiElement() {
	lua_State* luaState = luaL_newstate();
	luaL_openlibs(luaState); // get all libs in state (math, os, io)

	std::string filename = config::MGE_LUA_PATH + "main.lua";
	luaL_dofile(luaState, filename.c_str()); //execute lua file
}

UiElement::~UiElement() {

}
