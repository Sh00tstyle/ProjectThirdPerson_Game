#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <vector>
#include "mge/behaviours/AbstractBehaviour.hpp"
#include "mge/eventSystem/SystemEventListener.hpp"

#pragma once
class SystemEventDispatcher
{
public:
	SystemEventDispatcher();
	~SystemEventDispatcher();

	static void SendKeyEvent(sf::Event pEvent); 
	static void AddListener( SystemEventListener* pListener); 
	static void RemoveListener();
private: 
	static std::vector<SystemEventListener*> _listenerVector; 

};

