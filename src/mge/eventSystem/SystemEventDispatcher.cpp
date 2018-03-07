#include "mge/eventSystem/SystemEventDispatcher.hpp"
#include <iostream>

std::vector<SystemEventListener*> SystemEventDispatcher::_listenerVector = std::vector<SystemEventListener*>();
std::map<std::string, SystemEventListener*> SystemEventDispatcher::_listenerMap = std::map<std::string, SystemEventListener*>();
int SystemEventDispatcher::_listenerCount; 

SystemEventDispatcher::SystemEventDispatcher()
{
}

SystemEventDispatcher::~SystemEventDispatcher()
{
}

void SystemEventDispatcher::SendKeyEvent(sf::Event pEvent)
{
	std::cout << "Key Event Called " << std::endl; 
	
	for (auto sysListener : _listenerMap)
	{
		//std::cout << sysListener.second << std::endl; 
		if(sysListener.second != nullptr) sysListener.second->onNotify(pEvent); 
	}

	/*
	for (unsigned i = 0; i < _listenerVector.size(); i++)
	{
		_listenerVector[i]->onNotify(pEvent); 
	}
	*/
}

void SystemEventDispatcher::AddListener(SystemEventListener* pListener, std::string pName)
{
	std::cout << pName + " added to Map  " << std::endl;
	_listenerMap[pName] = pListener;
	//_listenerMap.insert(std::make_pair(pName, pListener)); 
	//_listenerVector.push_back(pListener); 
}

void SystemEventDispatcher::RemoveListener(std::string pName)
{
	std::cout << pName << " Removed to Map  " << std::endl;

	//std::map<std::string, SystemEventListener*>::iterator it = _listenerMap.find(pName);
	//	_listenerMap.erase(pName);

	_listenerMap[pName] = nullptr;
}

void SystemEventDispatcher::ClearListenerMap()
{
	_listenerMap.clear();
}




/*
void SystemEventDispatcher::Register(GridMovementBehavior* pBehaviour)
{
	_behaviours.push_back(pBehaviour); 
}
*/

