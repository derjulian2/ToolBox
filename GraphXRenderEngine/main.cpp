//////////////////////////////////////////////////
// Example Code for GraphX Render-Engine        //
//////////////////////////////////////////////////
#include "GraphX/Core/Window.hpp"
//////////////////////////////////////////////////
int main(int argc, char** argv)
{
	// WINDOW SETUP
	grx::Window window(static_cast<uint32_t>(1920.0/1.5), static_cast<uint32_t>(1080.0/1.5));
	grx::Camera camera(glm::vec3(0.0f, 6.0f, 0.0f));
	camera.setSpeed(2.0f);
	// SHADER SETUP
	grx::Shader vertex_shader("GraphX/Graphics/vertexshader.vs", grx::Shader::ShaderType::VERTEX_SHADER);
	grx::Shader fragment_shader("GraphX/Graphics/fragmentshader.fs", grx::Shader::ShaderType::FRAGMENT_SHADER);
	grx::ShaderProgram program({ vertex_shader, fragment_shader });

	grx::Texture wall("C:/Users/Julia/Desktop/assets/models/wall.jpg");
	grx::Texture cobble("C:/Users/Julia/Desktop/assets/models/cobblestone.jpg");
	grx::Texture metal("C:/Users/Julia/Desktop/assets/models/metal.jpg");

	grx::CubeShape cube_1(glm::vec3(-3.0f, 3.0f, 3.0));
	grx::CubeShape cube_2(glm::vec3(3, 3, 3));
	cube_1.setTexture(metal);
	cube_2.setTexture(cobble);

	grx::Model monkey("C:/Users/Julia/Desktop/assets/models/monkey.obj");
	monkey.setTexture(metal);
	monkey.setPos(glm::vec3(-20.0f, 5.0f, 0.0f));
	monkey.setRotation(glm::vec3(0.0f, -90.0f, 0.0f));
	grx::Model world("C:/Users/Julia/Desktop/assets/models/world.obj");
	world.setTexture(wall);

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

		cube_1.setRotation(glm::vec3(glfwGetTime() * 5, 0.0f, 0.0f));
		cube_1.setScale(glm::vec3(abs(sin(glfwGetTime())) * 2.0f, 1.0f, 1.0f));
	
		cube_2.setPos(glm::vec3(0.0f, abs(sin(glfwGetTime())) * 2.0f, 0.0f));

		glm::vec2 offset = window.Mouse.getMoveOffset();
		glm::vec2 yaw_pitch = camera.getYawAndPitch();
		camera.setDir(yaw_pitch.x + offset.x, yaw_pitch.y + offset.y);

		window.clear(grx::Color(117, 187, 228, 220));
		
		window.draw(cube_1, program, camera);
		window.draw(cube_2, program, camera);

		window.draw(world, program, camera);
		window.draw(monkey, program, camera);

		window.timing.Stop();
	}

	return 0;
}
//////////////////////////////////////////////////