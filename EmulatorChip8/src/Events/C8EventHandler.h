#pragma once

#include "pch.h"
#include "C8EventType.h"

namespace Chip8
{
//	typedef void (*c8_event_listener)(C8EventType);

	using c8_event_listener = void(*)(C8EventType);

	class C8EventHandler
	{
	public:
		C8EventHandler();

		void invoke(C8EventType type) const;

		void remove_listener(const c8_event_listener& listener);

		void add_listener(const c8_event_listener& listener);

		inline C8EventType get_event_type() const { return event_type; }

	private:
		std::vector<c8_event_listener> event_listeners;
		C8EventType event_type;
	};
}