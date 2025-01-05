//////////////////////////////////////////////////
/// THULIUM LIBRARY RESOURCES HEADER FILE      ///
/// 2024 Julian Benzel                         ///
//////////////////////////////////////////////////
#ifndef THL_WINDOW_H
#define THL_WINDOW_H
//////////////////////////////////////////////////
#if __has_include("LinAlgpp.hpp")
#include "LinAlgpp.hpp"
#endif
//////////////////////////////////////////////////
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <cstdint>
#include <thread>
//////////////////////////////////////////////////
namespace thl
{
	//////////////////////////////////////////////////

	enum THLflags
	{
		THL_NO_BORDER
	};
	THLflags operator|(THLflags _f, THLflags __f);

	//////////////////////////////////////////////////

	enum THLcode
	{
		THL_OK,
		THL_ERROR,
		THL_OVERFLOW,
		THL_OUT_OF_RANGE,
		THL_SCENE_NOT_FOUND,
		THL_MISSING_SOURCE
	};
	THLcode operator|(THLcode _c, THLcode __c);

	//////////////////////////////////////////////////

	struct ColorTheme
	{
		vec4f gui_background;
		vec4f gui_outline;

		vec4f gui_background_buttonhover;
		vec4f gui_outline_buttonhover;
		vec4f gui_background_buttonclick;
		vec4f gui_outline_buttonclick;

		vec4f viewport_background;
		vec4f viewport_line_regular;
		vec4f viewport_line_axis;
	};

	struct DefaultColorThemes
	{
		ColorTheme default_bright;
		ColorTheme default_dark;
		ColorTheme default_exotic;
	};

	//////////////////////////////////////////////////

	typedef std::shared_ptr<std::jthread> ThreadHandler;

	//////////////////////////////////////////////////

	vec2f NDCtoScreenPosition(vec2f position, mat3f projection_matrix);
	vec2f WorldtoNDCPosition(vec2f position, mat3f projection_matrix);

	sf::Color assign(vec4f v);

	//////////////////////////////////////////////////

	class Window;

	class Font
	{
	public:
		Font(Window& window, std::string path);
		operator sf::Font& ();
	private:
		std::string path;
		sf::Font font;
	};

	class Texture
	{
	public:
		Texture(Window& window, std::string path);
		operator sf::Texture& ();
	private:
		std::string path;
		sf::Texture texture;
	};

	//////////////////////////////////////////////////

	class Surface
	{
	public:
		Surface(const uint16_t width, const uint16_t height, sf::RenderTarget& render_target);
	protected:
		void Refresh();

		uint16_t width;
		uint16_t height;
		mat3f ndc_to_screen_matrix;

		sf::RenderTarget& render_target;
	};

	class WindowSurface : protected Surface
	{
	public:
		WindowSurface(const uint16_t width, const uint16_t height, const std::string title);

	protected:
		friend class Scene;

		std::string title;
		sf::RenderWindow surface_window;
		sf::Event window_event;
	};

	class TextureSurface : protected Surface
	{
	public:
		TextureSurface(const uint16_t width, const uint16_t height);
	protected:
		sf::RenderTexture surface_texture;
	};

#if __has_include("ThuliumEvents.hpp")
#define THL_SCENE_HAS_EVENTS
#endif

#if __has_include("ThuliumWidgets.hpp")
#define THL_SCENE_HAS_WIDGETS
#endif

#ifdef THL_SCENE_HAS_EVENTS
	class ConstEvent;
	class KeyboardEvent;
	class MousebuttonEvent;
	class MouseEvent;
	class MousewheelEvent;
#endif

#ifdef THL_SCENE_HAS_WIDGETS
	class StaticBox;
	class StaticText;
	class Button;
#endif

	class Scene
	{
	public:
		Scene();
	private:
		friend Window;

		void QueryEvents(WindowSurface& surface);

#ifdef THL_SCENE_HAS_EVENTS
		friend ConstEvent;
		friend KeyboardEvent;
		friend MousebuttonEvent;
		friend MouseEvent;
		friend MousewheelEvent;

		std::vector<ConstEvent*> constevent_stack;
		std::vector<KeyboardEvent*> keyboardevent_stack;
		std::vector<MousebuttonEvent*> mousebuttonevent_stack;
		std::vector<MouseEvent*> mouseevent_stack;
		std::vector<MousewheelEvent*> mousewheelevent_stack;
#endif

#ifdef THL_SCENE_HAS_WIDGETS
		friend StaticBox;
		friend StaticText;
		friend Button;

		void RefreshWidgets(WindowSurface& surface);
		void RenderWidgets(WindowSurface& surface);

		std::vector<StaticBox*> staticbox_stack;
		std::vector<StaticText*> statictext_stack;
		std::vector<Button*> button_stack;
#endif

		vec4f background_color = vec4f(230.0f);
	};

	/*
	* \brief Window class for opening a GUI-window.
	* \brief --------------------
	* \param - width, height: dimensions of the window (can be resized if specified).
	* \param - title: the string to be displayed in the topbar of the window.
	*/
	class Window : private WindowSurface
	{
	public:
		Window(const uint16_t width, const uint16_t height, const std::string title);
		/*
		* \brief will order the window to display the passed scene and all of it's widgets.
		* \brief also activates all of the passed scene's events.
		* \brief --------------------
		* \param the scene you want to display.
		* \return exit/error code.
		*/
		THLcode BindScene(Scene& scene);
		/*
		* \brief check if window is currently displaying a bound scene.
		* \brief --------------------
		* \return true if a scene is bound, else false.
		*/
		bool HasBoundScene() const;
		/*
		* \brief closes this window. can be reopened again using thl::Window::Show().
		* \brief --------------------
		* \return exit/error code.
		*/
		THLcode Close();
		/*
		* \brief opens this window. can be closed using thl::Window::Close().
		* \brief --------------------
		* \return exit/error code.
		*/
		THLcode Show();
		/*
		* \brief [ EXPERIMENTAL | USE AT OWN RISK ]
		* \brief opens a GUI-window in another thread.
		* \brief --------------------
		* \brief - note that if you call thl::Window::ShowThreaded(), you cannot call thl::Window::Show() as you
		* \brief would try to open a single window in two threads.
		* \brief - it is encouraged to manage all GUI-related code in the main-thread. If you need multithreaded behaviour,
		* \brief execute the related code in a worker-thread
		* \return a pointer to the new thread, will join automatically when it goes out of scope.
		* \return do not discard this pointer.
		*/
		[[nodiscard]] ThreadHandler ShowThreaded();
	private:
		friend Font;
		friend Texture;

		THLcode Input();
		THLcode Update();
		THLcode Render();

		Scene* bound_scene = nullptr;

		std::vector<Font*> font_stack;
		std::vector<Texture*> texture_stack;
	};

	//////////////////////////////////////////////////

	class SubWindow : private thl::TextureSurface
	{
	public:
		SubWindow(const uint16_t width, const uint16_t height);

		Scene* bound_scene = nullptr;
	};

	//////////////////////////////////////////////////

	/*class Viewport2D
	{

	};

	class Viewport3D
	{

	};

	class Camera2D
	{
	public:
		Camera2D(vec2f position, float aspect_ratio, float fov);
		void Refresh();

		vec2f position;
		float aspect_ratio = 16.0f / 9.0f;
		float viewport_aspect_ratio = 0;
		float fov = 3.0f;

		mat3f view_matrix;
	};*/

};
//////////////////////////////////////////////////
#endif
//////////////////////////////////////////////////
