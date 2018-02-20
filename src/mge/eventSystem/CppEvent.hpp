#include "mge/eventSystem/EventHandlerBase.hpp"
#include "mge/eventSystem/EventHandler.hpp"
#include <map>

#pragma once
template <typename ReturnT, typename ParamT>
class CppEvent
{
	/*
	typedef std::map<int, EventHandlerBase<ReturnT, ParamT> *> HandlersMap;
	HandlersMap _handlers; 
	int _count; 
	*/

public:
	CppEvent()
		: _count(0) {}
	/*

	template <typename ListenerT>
	CppEventHandler attach(ListenerT * object, ReturnT(ListenerT::*member)(ParamT))
	{
		typedef ReturnT(ListenerT::*PtrMember)(ParamT);
		_handlers[_count] = (new EventHandler<ListenerT, ReturnT, ParamT>(object, member)); 
		_count++; 
		return _count - 1; 
	}

	bool detach(EventHandler id)
	{
		HandlersMap::iterator it = _handlers.find(id);

		if (it == _handlers.end())
			return false; 

		delete it->second; 
		_handlers.erase(it); 
		return true; 
	}

	ReturnT notify(ParamT pParam)
	{
		HandlersMap::iterator it = _handlers.begin(); 
		for (; it != _handlers.end(); it++)
		{
			it->second->notify(pParam); 
		}
		return true; 
	}
	*/
};