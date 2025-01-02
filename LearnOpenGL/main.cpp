#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb_image.h"
#include "shader.h"
#include "camera.h"

#include <iostream>

float WIDTH = 800.0f;
float HEIGHT = 600.0f;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

// create camera object
camera cam(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 45.0f, WIDTH, HEIGHT);

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
void process_input(GLFWwindow* window, float deltatime)
{
	cam.movement_keyboard(window, deltatime);
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	cam.movement_mouse(xpos, ypos);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	cam.movement_scroll(yoffset);
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "turtle_engine", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "failed to create glfw window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "failed to load GLAD" << std::endl;
		return -1;
	}
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

	Shader ourShader(
		"C:/Users/Julia/Desktop/visualstudioprojects/OpenGLlearnfile/OpenGLlearnfile/vertexshader.vs",
		"C:/Users/Julia/Desktop/visualstudioprojects/OpenGLlearnfile/OpenGLlearnfile/fragmentshader.fs"
	);

	float vertices[] = {
		// cube vertices of 6 faces
		// face consists of 2 triangles with diagonal going from top left to bottom right
		// vertices are defined clockwise on each triangle
		// texture is wrapped clockwise along the vertices
		// positions        // colors		// texture
		// front face
		 1,-1, 1,			1,0,0,			0,1,
		 1, 1, 1,			1,0,0,			1,1,
		 1, 1,-1,			1,0,0,			1,0,

		 1,-1, 1,			1,0,0,			0,1,
		 1, 1,-1,			1,0,0,			1,0,
		 1,-1,-1,			1,0,0,			0,0,
		 // right face
		  1, 1, 1,			0,1,0,			0,1,
		 -1, 1, 1,			0,1,0,			1,1,
		 -1, 1,-1,			0,1,0,			1,0,

		  1, 1, 1,			0,1,0,			0,1,
		 -1, 1,-1,			0,1,0,			1,0,
		  1, 1,-1,			0,1,0,			0,0,
		  // left face
		  -1,-1, 1,			0,0,1,			0,1,
		   1,-1, 1,			0,0,1,			1,1,
		   1,-1,-1,			0,0,1,			1,0,

		  -1,-1, 1,			0,0,1,			0,1,
		   1,-1,-1,			0,0,1,			1,0,
		  -1,-1,-1,			0,0,1,			0,0,
		  // back face
		  -1, 1, 1,			1,1,0,			0,1,
		  -1,-1, 1,			1,1,0,			1,1,
		  -1,-1,-1,			1,1,0,			1,0,

		  -1, 1, 1,			1,1,0,			0,1,
		  -1,-1,-1,			1,1,0,			1,0,
		  -1, 1,-1,			1,1,0,			0,0,
		  // top face
		  -1,-1, 1,			0,1,1,			0,1,
		  -1, 1, 1,			0,1,1,			1,1,
		   1, 1, 1,			0,1,1,			1,0,

		  -1,-1, 1,			0,1,1,			0,1,
		   1, 1, 1,			0,1,1,			1,0,
		   1,-1, 1,			0,1,1,			0,0,
		   // bottom face
		   -1,-1,-1,			1,1,1,			0,1,
			1, 1,-1,			1,1,1,			1,0,
		   -1, 1,-1,			1,1,1,			1,1,

		   -1,-1,-1,			1,1,1,			0,1,
			1,-1,-1,			1,1,1,			0,0,
			1, 1,-1,			1,1,1,			1,0,
	};

	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* data = stbi_load("C:/Users/Julia/Desktop/visualstudioprojects/OpenGLlearnfile/OpenGLlearnfile/wall.jpg", &width, &height, &nrChannels, 0);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);
	// render loop
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		// input processing
		process_input(window, deltaTime);

		// rendering commands here
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		cam.update(ourShader);

		glBindTexture(GL_TEXTURE_2D, texture);
		ourShader.use();

		glBindVertexArray(VAO);

		glDrawArrays(GL_TRIANGLES, 0, 36);

		// check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}
