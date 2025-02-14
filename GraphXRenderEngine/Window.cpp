//////////////////////////////////////////////////
#include "Window.hpp"
//////////////////////////////////////////////////
void grx::EventHandler::PollEvents(GLFWwindow* window)
{
	Keyboard.pressed_keys.clear();
	Mouse.pressed_buttons.clear();
	Mouse::xoffset = 0.0f;
	Mouse::yoffset = 0.0f;
	if (window != nullptr)
	{
		glfwSwapBuffers(window);
		glfwPollEvents();

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			Keyboard.pressed_keys.emplace_back(KeyBoard::Key::ESC);
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			Keyboard.pressed_keys.emplace_back(KeyBoard::Key::W);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			Keyboard.pressed_keys.emplace_back(KeyBoard::Key::A);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			Keyboard.pressed_keys.emplace_back(KeyBoard::Key::S);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			Keyboard.pressed_keys.emplace_back(KeyBoard::Key::D);
	}
}
//////////////////////////////////////////////////
bool grx::KeyBoard::isKeyPressed(Key key) const
{
	for (const Key& k : pressed_keys)
	{
		if (k == key)
			return true;
	}
	return false;
}
//////////////////////////////////////////////////
bool grx::Mouse::isButtonPressed(char button) const
{
	for (const uint32_t& b : pressed_buttons)
	{
		if (b == button)
			return true;
	}
	return false;
}
glm::vec2 grx::Mouse::getMoveOffset() const
{
	return glm::vec2(xoffset, yoffset);
}
//////////////////////////////////////////////////
grx::Window::Window(uint32_t width,
	uint32_t height,
	std::string title)
	: width(width), height(height), title(title)
{
	if (!glfwInit())
	{
		throw std::runtime_error("GLFW initialization unsuccessful");
	}
	window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		throw std::runtime_error("GLFW window creation unsuccessful");
	}
	else
	{
		Mouse::lastx = static_cast<float>(width / 2);
		Mouse::lasty = static_cast<float>(height / 2);
		glfwMakeContextCurrent(window);
		glfwSetFramebufferSizeCallback(window, grx::framebuffer_size_callback);
		glfwSetCursorPosCallback(window, grx::mouse_callback);
		
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		glfwTerminate();
		throw std::runtime_error("GLAD glLoader initialization unsuccessful");
	}
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, width, height);
}
grx::Window::~Window()
{
	glfwTerminate();
	window = nullptr;
}
bool grx::Window::isOpen()
{
	if (window != nullptr)
		return !glfwWindowShouldClose(window);
	return false;
}
void grx::Window::close()
{
	if (window != nullptr)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
		window = nullptr;
	}
}
void grx::Window::PollEvents()
{
	PollEvents(window);
}
void grx::Window::draw(Drawable& drawable, const ShaderProgram& program, Camera& camera) const
{
	glClear(GL_DEPTH_BUFFER_BIT);
	// projection matrix
	program.setUniform("projection_matrix", 
		glm::perspective(
			glm::radians(camera.getFov()), 
			static_cast<float>(width) / static_cast<float>(height), 
			0.1f, 100.0f));
	// view matrix
	camera.setMatrix(program);
	// model matrix
	glm::mat4 model_matrix = glm::mat4(1.0f);
	model_matrix = glm::translate(model_matrix, glm::vec3(1.0f, 1.0f, 1.0f));
	program.setUniform("model_matrix", model_matrix);

	program.use();
	drawable.draw();
}
void grx::Window::clear(const Color& color) const
{
	glClearColor(color.r, color.g,color.b,color.a);
	glClear(GL_COLOR_BUFFER_BIT);
}
//////////////////////////////////////////////////
void grx::Window::Timing::Start()
{
	current_frame = glfwGetTime();
}
void grx::Window::Timing::Stop()
{
	deltaTime = current_frame - last_frame;
	last_frame = current_frame;
}
//////////////////////////////////////////////////
void grx::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
void grx::mouse_callback(GLFWwindow* window, double xposin, double yposin)
{
	float xpos = static_cast<float>(xposin);
	float ypos = static_cast<float>(yposin);
	if (Mouse::firstMouse)
	{
		Mouse::lastx = xpos;
		Mouse::lasty = ypos;
		Mouse::firstMouse = false;
	}

	Mouse::xoffset = (xpos - Mouse::lastx) * Mouse::sensitivity;
	Mouse::yoffset = (Mouse::lasty - ypos) * Mouse::sensitivity;

	Mouse::lastx = xpos;
	Mouse::lasty = ypos;
}
//////////////////////////////////////////////////