#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <vector>
#include "mge/behaviours/AbstractBehaviour.hpp"
#include "mge/eventSystem/SystemEventListener.hpp"
#include "mge/eventSystem/CppEvent.hpp"
#include "mge/eventSystem/Subject.hpp"
#include "mge/eventSystem/Observer.hpp"

#pragma once
class SystemEventDispatcher
{
public:
	SystemEventDispatcher();
	~SystemEventDispatcher();

	static void SendKeyEvent(sf::Event pEvent); 
	 static void AddListener( SystemEventListener* pListener); 
private: 
	static std::vector<SystemEventListener*> _listenerVector; 

};

