#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"

class camera
{
public:
	camera(glm::vec3 pos, glm::vec3 front, glm::vec3 up, float fov, float width, float height);

	bool camera_state = true; // active/unactive camera
	bool firstMouse = true;
	glm::vec3 cam_position, cam_front, cam_up;
	glm::mat4 view_matrix, projection_matrix, model_matrix;
	float cam_fov, cam_aspect_ratio, clip_near, clip_far, cameraspeed, pitch, yaw, lastX, lastY;
	float unit_speed = 2.5f;

	void update(Shader shader);
	void movement_keyboard(GLFWwindow* window, float deltatime);
	void movement_mouse(float xpos, float ypos);
	void movement_scroll(float yoffset);
	
};
camera::camera(glm::vec3 pos, glm::vec3 front, glm::vec3 up, float fov, float width, float height)
{
	cam_position = pos;
	cam_front = front;
	cam_up = up;
	cam_fov = fov;
	cam_aspect_ratio = width / height;
	lastX = width / 2;
	lastY = height / 2;
	clip_near = 0.1f;
	clip_far = 100.0f;
	yaw = -90.0f;
	pitch = 0;
}
void camera::update(Shader shader)
{
	if (camera_state)
	{
		model_matrix = glm::mat4(1.0f);
		view_matrix = glm::lookAt(cam_position, cam_position + cam_front, cam_up);
		projection_matrix = glm::perspective(glm::radians(cam_fov), cam_aspect_ratio, clip_near, clip_far);


		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model_matrix));
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view_matrix));
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection_matrix));
	}
}
void camera::movement_keyboard(GLFWwindow* window, float deltatime)
{
	cameraspeed = unit_speed * deltatime;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cam_position += cameraspeed * cam_front;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cam_position -= glm::normalize(glm::cross(cam_front, cam_up)) * cameraspeed;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cam_position -= cameraspeed * cam_front;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cam_position += glm::normalize(glm::cross(cam_front, cam_up)) * cameraspeed;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		cam_position += cameraspeed * cam_up;
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		cam_position -= cameraspeed * cam_up;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		unit_speed = 5.0f;
	else
		unit_speed = 2.5f;
}
void camera::movement_mouse(float xpos, float ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;
	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;
	yaw += xoffset;
	pitch += yoffset;
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;
	glm::vec3 direction = glm::vec3(cos(glm::radians(yaw)) * cos(glm::radians(pitch)), sin(glm::radians(pitch)), sin(glm::radians(yaw)) * cos(glm::radians(pitch)));
	cam_front = glm::normalize(direction);
}
void camera::movement_scroll(float yoffset)
{
	cam_fov -= (float)yoffset;
	if (cam_fov < 1.0f)
		cam_fov = 1.0f;
	if (cam_fov > 45.0f)
		cam_fov = 45.0f;
}
