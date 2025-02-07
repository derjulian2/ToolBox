//////////////////////////////////////////////////
#ifndef GRX_EVENTS_H
#define GRX_EVENTS_H
//////////////////////////////////////////////////
#include "Primitives.hpp"
#include <optional>
#include <functional>
//////////////////////////////////////////////////
namespace grx
{
	class Event
	{
	public:
		virtual void queryevent(const std::optional<sf::Event>& sfevent, sf::Window& sfwindow) const;
	
		bool ACTIVE = true;
	};
	/*
	* Frame-Update Event which gets executed
	* in mainloop AFTER other events were queried
	* and BEFORE rendering
	*/
	class FrameEvent
	{
	public:
		FrameEvent(const std::function<void(void)>& func);
		void queryevent() const;
		bool ACTIVE = true;
	private:
		std::function<void(void)> event_function;
	};

	class MouseButtonEvent : public Event
	{
	public:
		MouseButtonEvent(const std::function<void(Position2D)>& func);
		void queryevent(const std::optional<sf::Event>& sfevent, sf::Window& sfwindow) const override;
	private:
		std::function<void(Position2D)> event_function;
	};

	class MouseWheelEvent : public Event
	{

	};

	class MouseMoveEvent : public Event
	{

	};

	class KeyBoardEvent : public Event
	{

	};
}
//////////////////////////////////////////////////
#endif
//////////////////////////////////////////////////