//////////////////////////////////////////////////
#ifndef GRX_WINDOW_H
#define GRX_WINDOW_H
//////////////////////////////////////////////////
#include <cstdint>
#include <string>
#include <stdexcept>
#include <vector>
#include "Drawables.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include <GLFW/glfw3.h>
//////////////////////////////////////////////////
namespace grx
{
	//////////////////////////////////////////////////
	class KeyBoard
	{
	public:
		enum Key
		{
			ESC,
			W,
			A,
			S,
			D
		};

		bool isKeyPressed(Key key) const;
	private:
		friend class EventHandler;
		std::vector<Key> pressed_keys;
	};

	struct Mouse
	{
	public:
		bool isButtonPressed(char button) const;
		glm::vec2 getMoveOffset() const;
	private:
		friend class EventHandler;
		friend class Window;
		friend void mouse_callback(GLFWwindow* window, double xpos, double ypos);
		
		static inline float xoffset = 0.0f;
		static inline float yoffset = 0.0f;

		static inline float lastx = 0.0f;
		static inline float lasty = 0.0f;
		
		static inline float sensitivity = 0.1f;

		static inline bool firstMouse = true;

		std::vector<uint32_t> pressed_buttons;
	};

	class EventHandler
	{
	public:
		EventHandler() = default;

		KeyBoard Keyboard;
		Mouse Mouse;


	protected:
		void PollEvents(GLFWwindow* window);
	};

	//////////////////////////////////////////////////

	class Window : public EventHandler
	{
	public:
		Window(uint32_t width = 500,
			uint32_t height = 500,
			std::string title = "GraphX");
		Window(const Window&) = delete;
		Window(const Window&&) = delete;
		Window& operator=(const Window&) = delete;
		const Window& operator=(const Window&&) = delete;
		~Window();

		bool isOpen();
		void close();
		void PollEvents();
		void draw(Drawable& drawable, 
			const ShaderProgram& program,
			Camera& camera) const;
		void clear(const Color& color) const;

		struct Timing
		{
			void Start();
			void Stop();
			float deltaTime = 0.0f;
		private:
			float current_frame = 0.0f;
			float last_frame = 0.0f;
		} timing;

	private:
		uint32_t width;
		uint32_t height;
		std::string title;
		GLFWwindow* window = nullptr;

		using EventHandler::PollEvents;
	};

	//////////////////////////////////////////////////

	void framebuffer_size_callback(GLFWwindow* window, int width, int height);

	void mouse_callback(GLFWwindow* window, double xposin, double yposin);

	//////////////////////////////////////////////////
}
//////////////////////////////////////////////////
#endif
//////////////////////////////////////////////////