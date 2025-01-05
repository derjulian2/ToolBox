//////////////////////////////////////////////////
/// THULIUM LIBRARY EVENTS HEADER FILE         ///
/// 2024 Julian Benzel                         ///
//////////////////////////////////////////////////
#ifndef THL_EVENTS_H
#define THL_EVENTS_H
//////////////////////////////////////////////////
#include <SFML/Graphics.hpp>
#include <functional>
//////////////////////////////////////////////////
namespace thl
{
	class Scene;

	class ConstEvent
	{
	public:
		ConstEvent(Scene& scene, std::function<void()> function);
	private:
		friend Scene;

		bool event_status = true;
		std::function<void()> event_function = NULL;
	};

	class KeyboardEvent
	{
	public:
		KeyboardEvent(Scene& scene, std::function<void()> function, sf::Keyboard::Key key);
		KeyboardEvent(Scene& scene, std::function<void()> function_active, std::function<void()> function_inactive, sf::Keyboard::Key key);
	private:
		friend Scene;

		bool event_status = false;
		bool holdable = false;
		std::function<void()> event_function_active = NULL;
		std::function<void()> event_function_inactive = NULL;
		sf::Keyboard::Key event_key = sf::Keyboard::Key::Escape;
	};

	class MousebuttonEvent
	{
	public:
		MousebuttonEvent(Scene& scene, std::function<void()> function, sf::Mouse::Button button);
		MousebuttonEvent(Scene& scene, std::function<void()> function_active, std::function<void()> function_inactive, sf::Mouse::Button button);

		void SetActiveFunction(std::function<void()> function);
		void SetInactiveFunction(std::function<void()> function);
	private:
		friend Scene;

		bool event_status = false;
		bool holdable = false;
		std::function<void()> event_function_active = NULL;
		std::function<void()> event_function_inactive = NULL;
		sf::Mouse::Button event_button = sf::Mouse::Button::Left;
	};

	class MouseEvent
	{
	public:
		MouseEvent(Scene& scene, std::function<void(float, float)> function);

		void SetFunction(std::function<void(float, float)> function);
	private:
		friend Scene;

		bool event_status = false;
		std::function<void(float, float)> event_function = NULL;
	};

	class MousewheelEvent
	{
	public:
		MousewheelEvent(Scene& scene, std::function<void(float)> function, int direction);
	private:
		friend Scene;

		bool event_status = false;
		std::function<void(float)> event_function = NULL;
		int event_direction;
	};
};
//////////////////////////////////////////////////
#endif
//////////////////////////////////////////////////