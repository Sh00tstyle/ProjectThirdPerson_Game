#include "mge/eventSystem/Subject.hpp"

Subject::Subject() {}

Subject::~Subject() {}

/*
void Subject::addHandler(Observer& pObserver)
{
	_observers->push_back(pObserver); 
}

void Subject::removeHandler(Observer& pObserver)
{
	
	std::vector<Observer>::iterator it = _observers->begin();
	for (; it != _observers->end(); it++)
	{
		if (it == pObserver)
		{

		}
	}
	
}

void Subject::notify(sf::Event pEvent)
{
	std::vector<Observer>::iterator it = _observers->begin();
	for (; it != _observers->end(); it++)
	{
		it->onNotify(pEvent); 
	}
}
*/
