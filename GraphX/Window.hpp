//////////////////////////////////////////////////
#ifndef GRX_WINDOW_H
#define GRX_WINDOW_H
//////////////////////////////////////////////////
#include <SFML/Graphics.hpp>
//////////////////////////////////////////////////
#include "Primitives.hpp"
#include "Widgets.hpp"
//////////////////////////////////////////////////
#include <cstdint>
#include <string>
#include <vector>
//////////////////////////////////////////////////
namespace grx
{

	class Scene // will later be used in window instance as 'active scene'
	{
	public:
		std::vector<const Event*> events;
		std::vector<const Drawable*> drawables;
	};

	class Window
	{
	public:
		Window(uint32_t width = 500,
			uint32_t height = 500,
			const std::string& title = "GraphX");

		void draw(const Drawable& drawable);
		void addevent(const Event& event);
		void addevent(const FrameEvent& fevent);

		void display();

	private:
		
		uint32_t width;
		uint32_t height;
		std::string title;
		sf::RenderWindow sfml_window;

		std::vector<const Event*> events;
		std::vector<const FrameEvent*> frame_events;
		std::vector<const Drawable*> drawables;
	};

}
//////////////////////////////////////////////////
#endif
//////////////////////////////////////////////////