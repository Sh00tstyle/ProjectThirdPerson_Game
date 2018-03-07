#include "mge/managers/InputManager.h"

//default states
bool InputManager::_menuInput = true;
bool InputManager::_gameInput = false;

bool InputManager::GetMenuInput() {
	return _menuInput;
}

bool InputManager::GetGameInput() {
	return _gameInput;
}

void InputManager::SetMenuInput(bool value) {
	_menuInput = value;
}

void InputManager::SetGameInput(bool value) {
	_gameInput = value;
}