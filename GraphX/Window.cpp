//////////////////////////////////////////////////
#include "Window.hpp"
//////////////////////////////////////////////////
grx::Window::Window(uint32_t width,
	uint32_t height,
	const std::string& title)
	: width(width), height(height), title(title)
{

}

void grx::Window::draw(const Drawable& drawable)
{
	drawables.emplace_back(&drawable);
}

void grx::Window::addevent(const Event& event)
{
	events.emplace_back(&event);
}

void grx::Window::addevent(const FrameEvent& fevent)
{
	frame_events.emplace_back(&fevent);
}

void grx::Window::display()
{
	sf::ContextSettings settings;
	settings.antiAliasingLevel = 16;
	sfml_window.create(sf::VideoMode({ width, height }), title, sf::State::Windowed, settings);

	while (sfml_window.isOpen())
	{
		// event query stage
		while (const std::optional<sf::Event> sf_event = sfml_window.pollEvent())
		{
			if (sf_event->is<sf::Event::Closed>())
				sfml_window.close();
			for (const Event* e : events)
			{
				if (e->ACTIVE)
					e->queryevent(sf_event, sfml_window);
			}
		}

		// updating stage
		for (const FrameEvent* e : frame_events)
		{
			if (e->ACTIVE)
				e->queryevent();
		}

		// rendering stage
		sfml_window.clear(sf::Color(51, 77, 77, 255));

		for (const Drawable* d : drawables)
			if (d->SHOW)
				d->drawtotarget(sfml_window);

		sfml_window.display();
	}
}