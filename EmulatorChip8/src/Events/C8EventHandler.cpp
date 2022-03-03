#include "C8EventHandler.h"
#include "C8EventType.h"

namespace Chip8
{
	C8EventHandler::C8EventHandler()
	{
		event_listeners = std::vector<c8_event_listener>();
	}

	void C8EventHandler::add_listener(const c8_event_listener& listener)
	{
		event_listeners.push_back(listener);
	}

	void C8EventHandler::remove_listener(const c8_event_listener& listener)
	{
		std::vector<c8_event_listener>::iterator it = event_listeners.begin();

		while (it != event_listeners.end())
		{
			if (*it == listener)
				it = event_listeners.erase(it);
			else
				it += 1;
		}
	}

	void C8EventHandler::invoke(C8EventType type) const
	{
		std::vector<c8_event_listener>::const_iterator it = event_listeners.begin();

		while (it != event_listeners.end())
		{
			(*it)(type);
			it += 1;
		}
	}
}