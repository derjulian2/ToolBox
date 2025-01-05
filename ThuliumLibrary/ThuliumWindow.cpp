//////////////////////////////////////////////////
/// THULIUM LIBRARY WINDOWS SOURCE FILE        ///
/// 2024 Julian Benzel						   ///
//////////////////////////////////////////////////
#if __has_include("ThuliumWindow.hpp")
#include "ThuliumWindow.hpp"
#endif
#if __has_include("ThuliumWidgets.hpp")
#include "ThuliumWidgets.hpp"
#endif
#if __has_include("ThuliumEvents.hpp")
#include "ThuliumEvents.hpp"
#endif
//////////////////////////////////////////////////
#ifdef THL_WINDOW_H
//////////////////////////////////////////////////
thl::THLflags thl::operator|(THLflags _f, THLflags __f)
{
	return static_cast<THLflags>((uint16_t)_f | (uint16_t)__f);
}

//////////////////////////////////////////////////

thl::THLcode thl::operator|(THLcode _c, THLcode __c)
{
	return static_cast<THLcode>((uint16_t)_c | (uint16_t)__c);
}

//////////////////////////////////////////////////

thl::Font::Font(Window& window, std::string path)
	: path(path)
{
	bool font_unique = false;
	for (Font* font : window.font_stack)
		if (font->path == path)
			font_unique = true;

	if (!font_unique)
	{
		font.loadFromFile(path);
		window.font_stack.emplace_back(this);
	}
}
thl::Font::operator sf::Font& ()
{
	return font;
}
thl::Texture::Texture(Window& window, std::string path)
{
	bool texture_unique = false;
	for (Texture* texture : window.texture_stack)
		if (texture->path == path)
			texture_unique = true;

	if (!texture_unique)
	{
		texture.loadFromFile(path);
		window.texture_stack.emplace_back(this);
	}
}
thl::Texture::operator sf::Texture& ()
{
	return texture;
}

//////////////////////////////////////////////////

thl::Surface::Surface(const uint16_t width, const uint16_t height, sf::RenderTarget& render_target)
	: width(width), height(height), render_target(render_target)
{

}

void thl::Surface::Refresh()
{
	ndc_to_screen_matrix = mat3f(1.0);

	ndc_to_screen_matrix = translate(ndc_to_screen_matrix, vec2f(0.5f * float(width), 0.5f * float(height)));
	ndc_to_screen_matrix = scale(ndc_to_screen_matrix, vec2f(0.5f * float(width), 0.5f * float(height)));
}

thl::WindowSurface::WindowSurface(const uint16_t width, const uint16_t height, const std::string title)
	: title(title), Surface(width, height, surface_window)
{

}

thl::TextureSurface::TextureSurface(const uint16_t width, const uint16_t height)
	: Surface(width, height, surface_texture)
{

}
//////////////////////////////////////////////////

vec2f thl::NDCtoScreenPosition(vec2f position, mat3f projection_matrix)
{
	vec3f position_projected = projection_matrix * vec3f(position.x, position.y, 1.0f);
	return vec2f(position_projected.x, position_projected.y);
}
vec2f thl::WorldtoNDCPosition(vec2f position, mat3f projection_matrix)
{
	vec3f position_projected = projection_matrix * vec3f(position.x, position.y, 1.0f);
	return vec2f(position_projected.x, position_projected.y);
}

sf::Color thl::assign(vec4f v)
{
	return sf::Color(static_cast<uint8_t>(v.x), static_cast<uint8_t>(v.y), static_cast<uint8_t>(v.z), static_cast<uint8_t>(v.w));
}
//////////////////////////////////////////////////
thl::Scene::Scene()
{

}
void thl::Scene::QueryEvents(WindowSurface& surface)
{
	while (surface.surface_window.pollEvent(surface.window_event))
	{
		if (surface.window_event.type == sf::Event::Closed)
			surface.surface_window.close();
		if (surface.window_event.type == sf::Event::Resized)
		{
			sf::FloatRect visibleArea(0, 0, surface.window_event.size.width, surface.window_event.size.height);
			surface.surface_window.setView(sf::View(visibleArea));
			surface.width = surface.window_event.size.width;
			surface.height = surface.window_event.size.height;
		}
#ifdef THL_SCENE_HAS_EVENTS
		if (surface.window_event.type == sf::Event::KeyPressed)
		{
			for (KeyboardEvent* keyboardevent : keyboardevent_stack)
			{
				if (keyboardevent->event_function_active != NULL && surface.window_event.key.code == keyboardevent->event_key)
					keyboardevent->event_function_active();
			}
		}
		if (surface.window_event.type == sf::Event::KeyReleased)
		{
			for (KeyboardEvent* keyboardevent : keyboardevent_stack)
			{
				if (keyboardevent->event_function_inactive != NULL && surface.window_event.key.code == keyboardevent->event_status)
					keyboardevent->event_function_inactive();
			}
		}
		if (surface.window_event.type == sf::Event::MouseButtonPressed)
		{
			for (MousebuttonEvent* mouseevent : mousebuttonevent_stack)
			{
				if (mouseevent->event_function_active != NULL && surface.window_event.mouseButton.button == mouseevent->event_button)
					mouseevent->event_function_active();
			}
		}
		if (surface.window_event.type == sf::Event::MouseButtonReleased)
		{
			for (MousebuttonEvent* mouseevent : mousebuttonevent_stack)
			{
				if (mouseevent->event_function_inactive != NULL && surface.window_event.mouseButton.button == mouseevent->event_button)
					mouseevent->event_function_inactive();
			}
		}
		if (surface.window_event.type == sf::Event::MouseMoved)
		{
			for (MouseEvent* mouseevent : mouseevent_stack)
			{
				if (mouseevent->event_function != NULL)
					mouseevent->event_function(sf::Mouse::getPosition(surface.surface_window).x, sf::Mouse::getPosition(surface.surface_window).y);
			}
		}
		if (surface.window_event.type == sf::Event::MouseWheelScrolled)
		{
			for (MousewheelEvent* mousewheelevent : mousewheelevent_stack)
			{
				if (mousewheelevent->event_function != NULL && surface.window_event.mouseWheelScroll.delta == mousewheelevent->event_direction)
					mousewheelevent->event_function(surface.window_event.mouseWheelScroll.delta);
			}
		}
#endif
	}
#ifdef THL_SCENE_HAS_EVENTS
	for (ConstEvent* constevent : constevent_stack)
	{
		if (constevent->event_function != NULL)
			constevent->event_function();
	}
#endif

}
#ifdef THL_SCENE_HAS_WIDGETS
void thl::Scene::RefreshWidgets(WindowSurface& surface)
{
	for (StaticBox* box : staticbox_stack)
		box->Refresh(surface.ndc_to_screen_matrix);
	for (StaticText* text : statictext_stack)
		text->Refresh(surface.ndc_to_screen_matrix);
#ifdef THL_SCENE_HAS_EVENTS
	for (Button* button : button_stack)
		button->Refresh(surface.ndc_to_screen_matrix);
#endif
}

void thl::Scene::RenderWidgets(WindowSurface& surface)
{
	for (StaticBox* box : staticbox_stack)
	{
		surface.render_target.draw(box->sfml_shape);
		surface.render_target.draw(box->sfml_sprite);
	}
	for (StaticText* text : statictext_stack)
	{
		surface.render_target.draw(text->sfml_text);
	}
#ifdef THL_SCENE_HAS_EVENTS
	for (Button* button : button_stack)
	{
		surface.render_target.draw(button->sfml_shape);
		surface.render_target.draw(button->sfml_sprite);
		surface.render_target.draw(button->sfml_text);
	}
#endif
}
#endif
//////////////////////////////////////////////////
thl::Window::Window(const uint16_t width, const uint16_t height, const std::string title)
	: WindowSurface(width, height, title)
{

}

thl::THLcode thl::Window::BindScene(Scene& scene)
{
	try
	{
		bound_scene = &scene;
		// REMEMBER TO SWITCH OFF ALL EVENTS OF THE SCENE THAT WAS PREVIOUSLY BOUND (or detect, where BindScene() was used in Event)
		// scene-switches can and should happen by events
		// if another scene switches back through an event, the old event will be invoked ending in
		// an infinite scene-switching-loop -> should be avoided!
	}
	catch (std::exception exc)
	{
		return THL_ERROR;
	}
	return THL_OK;
}

bool thl::Window::HasBoundScene() const
{
	if (bound_scene != nullptr)
		return true;
	return false;
}

thl::THLcode thl::Window::Close()
{
	try
	{
		surface_window.close();
	}
	catch (std::exception exc)
	{
		std::cerr << "Exception thrown in thl::Quit(): " << exc.what() << ".\n";
		return THL_ERROR;
	}
	return THL_OK;
}

//////////////////////////////////////////////////

thl::THLcode thl::Window::Input()
{
	bound_scene->QueryEvents(*this);

	return THL_OK;
}
//////////////////////////////////////////////////
thl::THLcode thl::Window::Update()
{
	// important for window resizes
	//sf::Vector2u target_SIZE = window.surface_window->getSize();
	//window.width = target_SIZE.x;
	//window.height = target_SIZE.y;
	try
	{
		Refresh();
		#ifdef THL_SCENE_HAS_WIDGETS
		bound_scene->RefreshWidgets(*this);
		#endif
	}
	catch (std::exception exc)
	{
		std::cerr << "Exception thrown in thl::Update(): " << exc.what() << ".\n";
		return THL_ERROR;
	}
	return THL_OK;
}
thl::THLcode thl::Window::Render()
{
	try
	{
		surface_window.clear(assign(bound_scene->background_color));

		#ifdef THL_SCENE_HAS_WIDGETS
		bound_scene->RenderWidgets(*this);
		#endif

		surface_window.display();
	}
	catch (std::exception exc)
	{
		std::cerr << "Exception thrown in thl::Render(): " << exc.what() << ".\n";
		return THL_ERROR;
	}
	return THL_OK;
}
//////////////////////////////////////////////////
thl::THLcode thl::Window::Show() {
	try
	{
		Scene* defaultscene = nullptr;
		if (!HasBoundScene()) // generates an empty scene if no scene is bound at the time the window is called to show
		{
			defaultscene = new Scene();
			BindScene(*defaultscene);
		}
		surface_window.create(sf::VideoMode(width, height), title);
		while (surface_window.isOpen())
		{

			Input();
			Update();
			Render();
		}
		Close();
		if (defaultscene != nullptr)
			delete defaultscene;
	}
	catch (std::exception exc)
	{
		std::cerr << "Exception thrown in thl::Show(): " << exc.what() << ".\n";
		return THL_ERROR;
	}
	return THL_OK;
}
thl::ThreadHandler thl::Window::ShowThreaded()
{
	return std::make_shared<std::jthread>([this]() -> THLcode
		{
			Show();
			return THL_OK;
		});
}
//////////////////////////////////////////////////
thl::SubWindow::SubWindow(const uint16_t width, const uint16_t height)
	: TextureSurface(width, height)
{

}
//////////////////////////////////////////////////
#endif
//////////////////////////////////////////////////