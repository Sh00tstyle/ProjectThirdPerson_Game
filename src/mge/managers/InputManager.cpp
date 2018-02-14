#include "InputManager.hpp"
#include <iostream>


InputManager::InputManager()
{
}


InputManager::~InputManager()
{
}

void InputManager::UpdateInput(sf::Keyboard::Key pKey)
{
	if (pKey == sf::Keyboard::W)
		std::cout << "I only get displayed once"; 
}
