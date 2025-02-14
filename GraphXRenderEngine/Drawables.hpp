//////////////////////////////////////////////////
#ifndef GRX_DRAWABLES_H
#define GRX_DRAWABLES_H
//////////////////////////////////////////////////
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <stdexcept>
#include <array>
#include "Texture.hpp"
#include "VertexManagment.hpp"
//////////////////////////////////////////////////
namespace grx
{
	//////////////////////////////////////////////////
	class Window;

	class Drawable
	{
	public:
		Drawable() = default;
		virtual ~Drawable() = default;
	protected:
		friend Window;
		virtual void draw() const = 0;
		VertexBuffer buffer;
	};

	//////////////////////////////////////////////////

	class TriangleShape : public Drawable
	{
	public:
		TriangleShape() = default;
		TriangleShape(Vertex v1, Vertex v2, Vertex v3);

		void setVertices(Vertex v1, Vertex v2, Vertex v3);
		void draw() const override;
	private:
		std::array<Vertex, 3> vertices;
		VertexArray vertex_array;
	};

	class CubeShape : public Drawable
	{
	public:
		CubeShape(glm::vec3 position = glm::vec3(0, 0, 0),
			float size = 1.0f,
			Color color = Color(255.0f, 255.0f, 255.0f, 255.0f));

		glm::vec3 getPos() const;
		float getSize() const;
		Color getColor() const;

		void setSize(float size);
		void setPos(glm::vec3 pos);
		void setColor(Color color);
		void setTexture(Texture& tex);

		void draw() const override;
	private:
		void setVertices();
		glm::vec3 pos;
		float size;
		Color color;

		std::array<Vertex, 36> vertices;
		VertexArray vertex_array;
		VertexBuffer vertex_buffer;
		std::optional<std::reference_wrapper<Texture>> texture;
	};

	//////////////////////////////////////////////////

	class Mesh : public Drawable
	{

	};

	class Model : public Drawable
	{

	};

	//////////////////////////////////////////////////
}
//////////////////////////////////////////////////
#endif
//////////////////////////////////////////////////