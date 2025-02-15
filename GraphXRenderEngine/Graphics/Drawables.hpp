//////////////////////////////////////////////////
#ifndef GRX_DRAWABLES_H
#define GRX_DRAWABLES_H
//////////////////////////////////////////////////
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <stdexcept>
#include <array>
#include <list>
#include "Texture.hpp"
#include "../Core/VertexManagment.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
//////////////////////////////////////////////////
namespace grx
{
	//////////////////////////////////////////////////
	class Window;

	class Transformable
	{
	public:
		Transformable() = default;

		void setPos(glm::vec3 pos);
		void setScale(glm::vec3 scale);
		void setRotation(glm::vec3 rotation);

		glm::vec3 getPos() const;
		glm::vec3 getScale() const;
		glm::vec3 getRotation() const;

	protected:
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	};

	class Drawable : public Transformable
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
	private:
		void draw() const override;
		std::array<Vertex, 3> vertices;
		VertexArray vertex_array;
	};

	class CubeShape : public Drawable
	{
	public:
		CubeShape(glm::vec3 position = glm::vec3(0, 0, 0),
			float size = 1.0f,
			Color color = Color(255.0f, 255.0f, 255.0f, 255.0f));

		Color getColor() const;

		void setColor(Color color);
		void setTexture(Texture& tex);

	private:
		void draw() const override;
		void update();
		Color color;

		std::array<Vertex, 36> vertices;
		VertexArray vertex_array;
		VertexBuffer vertex_buffer;
		IndexBuffer index_buffer;
		std::optional<std::reference_wrapper<Texture>> texture;
	};

	//////////////////////////////////////////////////

	class Mesh
	{
	public:
		Mesh(const std::vector<Vertex>& vertices,
			const std::vector<uint32_t>& indices);
	private:
		friend class Model;
		void setBuffers();

		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
		VertexArray vertex_array;
		VertexBuffer vertex_buffer;
		IndexBuffer index_buffer;
	};

	class Model : public Drawable
	{
	public:
		Model(const std::filesystem::path& path);

		void setTexture(Texture& tex);
	private:
		void draw() const override;
		std::list<Mesh> meshes;
		std::optional<std::reference_wrapper<Texture>> texture;

		void processNode(aiNode* node, const aiScene* scene);
		Mesh toMesh(aiMesh* mesh, const aiScene* scene);
	};

	//////////////////////////////////////////////////
}
//////////////////////////////////////////////////
#endif
//////////////////////////////////////////////////