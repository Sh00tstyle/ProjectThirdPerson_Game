#include<SFML/Window/Event.hpp>
//#include "mge/eventSystem/CppEvent.hpp"
#pragma once

class SystemEventListener
{
public:
	SystemEventListener();
	virtual ~SystemEventListener();

	virtual void onNotify(sf::Event pEvent) = 0; 

};

