#ifndef INPUTMANAGER_HPP
#define INPUTMANAGER_HPP

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

class InputManager
{
public:
	InputManager();
	~InputManager();

	void UpdateInput(sf::Keyboard::Key pKey); 

private:
	enum Direction {Up, Left, Down, Right};
	Direction direction; 

};
#endif // !INPUTMANAGER_HPP;

