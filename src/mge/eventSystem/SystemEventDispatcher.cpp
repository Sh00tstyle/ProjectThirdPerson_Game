#include "mge/eventSystem/SystemEventDispatcher.hpp"

std::vector<SystemEventListener*> SystemEventDispatcher::_listenerVector = std::vector<SystemEventListener*>();

SystemEventDispatcher::SystemEventDispatcher()
{
}

SystemEventDispatcher::~SystemEventDispatcher()
{
}

/*
void SystemEventDispatcher::DispactchEvent(sf::Event pEvent)
{

	if (_behaviours.capacity != nullptr) {
		for (unsigned i = 0; i < _behaviours.capacity - 1; i++)
		{
			_behaviours[i]->HandleInput(pEvent.key.code); 
		}
	}
}
*/

void SystemEventDispatcher::SendKeyEvent(sf::Event pEvent)
{
	for (unsigned i = 0; i < _listenerVector.size(); i++)
	{
		_listenerVector[i]->onNotify(pEvent); 
	}
}

void SystemEventDispatcher::AddListener(SystemEventListener* pListener)
{
	_listenerVector.push_back(pListener); 
}



/*
void SystemEventDispatcher::Register(GridMovementBehavior* pBehaviour)
{
	_behaviours.push_back(pBehaviour); 
}
*/

