//////////////////////////////////////////////////
#ifndef GRX_CAMERA_H
#define GRX_CAMERA_H
//////////////////////////////////////////////////
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.hpp"
//////////////////////////////////////////////////
namespace grx
{
	//////////////////////////////////////////////////#

	class Camera
	{
	public:
		Camera(glm::vec3 position, float fov = 45.0f);

		/*
		* sets a uniform called 'view_matrix' in the passed shaderprogram
		* to its current view matrix
		*/
		void setMatrix(const ShaderProgram&) const;

		glm::vec3 getPos() const;
		glm::vec3 getDir() const;
		glm::vec2 getYawAndPitch() const;
		glm::vec3 getUp() const;
		float getFov() const;
		float getSpeed() const;

		void setPos(glm::vec3 pos);
		void setDir(float yaw, float pitch);
		void setSpeed(float speed);
	private:
		void calculateMatrix();

		glm::vec3 pos;
		glm::vec3 dir;
		glm::vec3 up;
		glm::vec3 right;

		float yaw = -90.0f;
		float pitch = 0.0f;
		float fov;
		float speed = 1.0f;

		glm::mat4 view_matrix;
	};

	//////////////////////////////////////////////////
}
//////////////////////////////////////////////////
#endif
//////////////////////////////////////////////////