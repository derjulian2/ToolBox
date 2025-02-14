//////////////////////////////////////////////////
// Example Code for GraphX Render-Engine        //
//////////////////////////////////////////////////
#include "Window.hpp"
//////////////////////////////////////////////////
int main(int argc, char** argv)
{
	// WINDOW SETUP
	grx::Window window(static_cast<uint32_t>(1920.0/1.5), static_cast<uint32_t>(1080.0/1.5));
	grx::Camera camera(glm::vec3(3.0f, 3.0f, 3.0f));
	camera.setSpeed(2.0f);
	// SHADER SETUP
	grx::Shader vertex_shader("C:/Users/Julia/Desktop/visualstudioprojects/GraphXRenderEngine/GraphXRenderEngine/vertexshader.vs", grx::Shader::ShaderType::VERTEX_SHADER);
	grx::Shader fragment_shader("C:/Users/Julia/Desktop/visualstudioprojects/GraphXRenderEngine/GraphXRenderEngine/fragmentshader.fs", grx::Shader::ShaderType::FRAGMENT_SHADER);
	grx::ShaderProgram program({ vertex_shader, fragment_shader });

	grx::CubeShape cube;
	grx::Texture texture("C:/Users/Julia/Desktop/resources/tilesets/cobblestone.jpg");
	cube.setTexture(texture);

	while (window.isOpen())
	{
		window.timing.Start();
		window.PollEvents();

		if (window.Keyboard.isKeyPressed(grx::KeyBoard::Key::ESC))
		{
			window.close();
		}
		if (window.Keyboard.isKeyPressed(grx::KeyBoard::Key::W))
		{
			camera.setPos(camera.getPos() + window.timing.deltaTime * camera.getSpeed() * camera.getDir());
		}
		if (window.Keyboard.isKeyPressed(grx::KeyBoard::Key::A))
		{
			camera.setPos(camera.getPos() - window.timing.deltaTime * camera.getSpeed() * glm::normalize(glm::cross(camera.getDir(), camera.getUp())));
		}
		if (window.Keyboard.isKeyPressed(grx::KeyBoard::Key::S))
		{
			camera.setPos(camera.getPos() - window.timing.deltaTime * camera.getSpeed() * camera.getDir());
		}
		if (window.Keyboard.isKeyPressed(grx::KeyBoard::Key::D))
		{
			camera.setPos(camera.getPos() + window.timing.deltaTime * camera.getSpeed() * glm::normalize(glm::cross(camera.getDir(), camera.getUp())));
		}

		glm::vec2 offset = window.Mouse.getMoveOffset();
		glm::vec2 yaw_pitch = camera.getYawAndPitch();
		camera.setDir(yaw_pitch.x + offset.x, yaw_pitch.y + offset.y);

		window.clear(grx::Color(51, 77, 77, 255));
		window.draw(cube, program, camera);

		window.timing.Stop();
	}

	return 0;
}
//////////////////////////////////////////////////