//////////////////////////////////////////////////
#include "Events.hpp"
//////////////////////////////////////////////////
void grx::Event::queryevent(const std::optional<sf::Event>& sfevent, sf::Window& sfwindow) const
{

}

grx::FrameEvent::FrameEvent(const std::function<void(void)>& func)
	: event_function(func)
{

}

void grx::FrameEvent::queryevent() const
{
	event_function();
}

grx::MouseButtonEvent::MouseButtonEvent(const std::function<void(Position2D)>& func)
	: event_function(func)
{

}

void grx::MouseButtonEvent::queryevent(const std::optional<sf::Event>& sfevent, sf::Window& sfwindow) const
{
	if (sfevent->is<sf::Event::MouseButtonPressed>())
		event_function({static_cast<float>(sf::Mouse::getPosition(sfwindow).x), static_cast<float>(sf::Mouse::getPosition(sfwindow).y)});
}
