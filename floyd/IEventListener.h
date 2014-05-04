#ifndef FLOYD_IEVENT_LISTENER_H
#define FLOYD_IEVENT_LISTENER_H


#include "Event.h"

class IEventListener
{
public:
	virtual ~IEventListener() {}

	virtual void OnEvent(const Event &_event) = 0;
};


#endif