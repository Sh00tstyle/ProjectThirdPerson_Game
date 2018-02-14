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
	//	std::cout << "I only get displayed once"; 
	if (pKey == sf::Keyboard::A)
		std::cout << "I go to the left"; 
	if (pKey == sf::Keyboard::S)
		std::cout << "I go down"; 
	if (pKey == sf::Keyboard::D)
		std::cout << "I go to the right"; 
}
