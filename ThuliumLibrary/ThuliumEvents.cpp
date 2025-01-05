//////////////////////////////////////////////////
/// THULIUM LIBRARY EVENTS - SOURCE FILE       ///
/// 2024 Julian Benzel                         ///
//////////////////////////////////////////////////
#if __has_include("ThuliumEvents.hpp")
#include "ThuliumEvents.hpp"
#endif
#if __has_include("ThuliumWindow.hpp")
#include "ThuliumWindow.hpp"
#endif
//////////////////////////////////////////////////
#if defined(THL_EVENTS_H) && defined(THL_WINDOW_H)
//////////////////////////////////////////////////
thl::ConstEvent::ConstEvent(Scene& scene, std::function<void()> function)
	: event_function(function)
{
	scene.constevent_stack.emplace_back(this);
}
//////////////////////////////////////////////////
thl::KeyboardEvent::KeyboardEvent(Scene& scene, std::function<void()> function, sf::Keyboard::Key key)
	: event_function_active(function), event_key(key)
{
	scene.keyboardevent_stack.emplace_back(this);
}
thl::KeyboardEvent::KeyboardEvent(Scene& scene, std::function<void()> function_active, std::function<void()> function_inactive, sf::Keyboard::Key key)
	: event_function_active(function_active), event_function_inactive(function_inactive), event_key(key)
{
	scene.keyboardevent_stack.emplace_back(this);
}
//////////////////////////////////////////////////
thl::MousebuttonEvent::MousebuttonEvent(Scene& scene, std::function<void()> function, sf::Mouse::Button button)
	: event_function_active(function), event_button(button)
{
	scene.mousebuttonevent_stack.emplace_back(this);
}
thl::MousebuttonEvent::MousebuttonEvent(Scene& scene, std::function<void()> function_active, std::function<void()> function_inactive, sf::Mouse::Button button)
	: event_function_active(function_active), event_function_inactive(function_inactive), event_button(button)
{
	scene.mousebuttonevent_stack.emplace_back(this);
}
void thl::MousebuttonEvent::SetActiveFunction(std::function<void()> function)
{
	event_function_active = function;
}
void thl::MousebuttonEvent::SetInactiveFunction(std::function<void()> function)
{
	event_function_inactive = function;
}
//////////////////////////////////////////////////
thl::MouseEvent::MouseEvent(Scene& scene, std::function<void(float, float)> function)
	: event_function(function)
{
	scene.mouseevent_stack.emplace_back(this);
}
void thl::MouseEvent::SetFunction(std::function<void(float, float)> function)
{
	event_function = function;
}
//////////////////////////////////////////////////
thl::MousewheelEvent::MousewheelEvent(Scene& scene, std::function<void(float)> function, int direction)
	: event_function(function), event_direction(direction)
{
	scene.mousewheelevent_stack.emplace_back(this);
}
//////////////////////////////////////////////////
#endif
//////////////////////////////////////////////////