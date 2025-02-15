//////////////////////////////////////////////////
#include "Camera.hpp"
//////////////////////////////////////////////////
grx::Camera::Camera(glm::vec3 pos, float fov)
	: pos(pos), up(glm::vec3(0.0f, 1.0f, 0.0f)), fov(fov)
{
	setDir(yaw, pitch);
	calculateMatrix();
}
void grx::Camera::calculateMatrix()
{
	right = glm::normalize(glm::cross(up, dir));
	view_matrix = glm::lookAt(
		pos,
		pos + dir,
		up
	);
}
void grx::Camera::setMatrix(const ShaderProgram& program) const
{
	program.setUniform("view_matrix", view_matrix);
}
//////////////////////////////////////////////////
glm::vec3 grx::Camera::getPos() const
{
	return pos;
}
glm::vec3 grx::Camera::getDir() const
{
	return dir;
}
glm::vec2 grx::Camera::getYawAndPitch() const
{
	return glm::vec2(yaw, pitch);
}
glm::vec3 grx::Camera::getUp() const
{
	return up;
}
float grx::Camera::getFov() const
{
	return fov;
}
float grx::Camera::getSpeed() const
{
	return speed;
}
//////////////////////////////////////////////////
void grx::Camera::setPos(glm::vec3 pos)
{
	this->pos = pos;
	calculateMatrix();
}
void grx::Camera::setDir(float yaw, float pitch)
{
	this->yaw = yaw;
	this->pitch = pitch;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	dir = glm::vec3(
		cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
	    sin(glm::radians(pitch)),
	    sin(glm::radians(yaw)) * cos(glm::radians(pitch))
		);
	dir = glm::normalize(dir);
	calculateMatrix();
}
void grx::Camera::setSpeed(float speed)
{
	this->speed = speed;
}
//////////////////////////////////////////////////