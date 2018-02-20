#include "mge/eventSystem/EventHandlerBase.hpp"

#pragma once
template <typename ListenerT, typename ReturnT, typename ParamT>
class EventHandler : public EventHandlerBase<ReturnT, ParamT>
{
	typedef ReturnT(ListenerT::*PtrMember)(ParamT);
	ListenerT* _object;
	PtrMember _member; 

public:
	EventHandler(ListenerT* object, ParamT param) //The Sender and the parameter to pass to the subscribed methods
		: _object(object), _member(member)
	{}

	ReturnT notify(ParamT param)
	{
		return (_object->*_member)(param); 
	}

};