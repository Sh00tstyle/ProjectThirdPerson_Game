#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <vector>
#include <map>
#include "mge/behaviours/AbstractBehaviour.hpp"
#include "mge/eventSystem/SystemEventListener.hpp"

#pragma once
class SystemEventDispatcher
{
public:
	SystemEventDispatcher();
	~SystemEventDispatcher();

	static void SendKeyEvent(sf::Event pEvent); 
	static void AddListener( SystemEventListener* pListener, std::string pName); 
	static void RemoveListener(std::string pName);
	static void ClearListenerMap(); 

private: 
	static std::vector<SystemEventListener*> _listenerVector; 
	static std::map<std::string,SystemEventListener*> _listenerMap; 
	static int _listenerCount; 
};

