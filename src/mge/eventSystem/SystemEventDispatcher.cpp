#include "mge/eventSystem/SystemEventDispatcher.hpp"
#include <iostream>

std::vector<SystemEventListener*> SystemEventDispatcher::_listenerVector = std::vector<SystemEventListener*>();

SystemEventDispatcher::SystemEventDispatcher()
{
}

SystemEventDispatcher::~SystemEventDispatcher()
{
}

void SystemEventDispatcher::SendKeyEvent(sf::Event pEvent)
{
	std::cout << "Key Event Called " << std::endl; 
	for (unsigned i = 0; i < _listenerVector.size(); i++)
	{
		_listenerVector[i]->onNotify(pEvent); 
	}
}

void SystemEventDispatcher::AddListener(SystemEventListener* pListener)
{
	std::cout << " added to Vector  " << std::endl;
	
	_listenerVector.push_back(pListener); 
}

void SystemEventDispatcher::RemoveListener()
{
	std::cout << "Vector cleared " << std::endl;
	_listenerVector.clear(); 
}



/*
void SystemEventDispatcher::Register(GridMovementBehavior* pBehaviour)
{
	_behaviours.push_back(pBehaviour); 
}
*/

