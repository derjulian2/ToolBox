//////////////////////////////////////////////////
#ifndef GRX_VERTEX_MANAGEMENT_H
#define GRX_VERTEX_MANAGEMENT_H
//////////////////////////////////////////////////
#include <cstdint>
#include <stdexcept>
#include <glad/glad.h>
#include <glm/glm.hpp>
//////////////////////////////////////////////////
namespace grx
{
	//////////////////////////////////////////////////

	struct Color
	{
		explicit Color(float r, float g, float b, float a);
		explicit Color(glm::vec4 rgba = glm::vec4(0, 0, 0, 0));
		float r;
		float g;
		float b;
		float a;
	};

	//////////////////////////////////////////////////

	struct Vertex
	{
		Vertex(glm::vec3 pos = glm::vec3(0, 0, 0),
			Color color = Color(glm::vec4(0, 0, 0, 0)),
			glm::vec2 texcoords = glm::vec2(0, 0));

		glm::vec3 pos;
		Color color;
		glm::vec2 texcoords;
	};

	//////////////////////////////////////////////////

	typedef unsigned int VertexArrayHandle;

	class VertexArray
	{
	public:
		VertexArray();
		VertexArray(const VertexArray&) = delete;
		VertexArray(const VertexArray&&) = delete;
		VertexArray& operator=(const VertexArray&) = delete;
		const VertexArray& operator=(const VertexArray&&) = delete;
		~VertexArray();

		VertexArrayHandle getHandle() const;
		void release();
	private:
		VertexArrayHandle vertex_array_handle;
	};

	typedef unsigned int IndexBufferHandle;

	class IndexBuffer
	{
	public:
		enum BufferMode
		{
			STREAM,
			STATIC,
			DYNAMIC
		};

		IndexBuffer();
		IndexBuffer(const VertexArray& vao,
			uint32_t* data,
			size_t size,
			BufferMode mode = STATIC);
		IndexBuffer(const IndexBuffer&) = delete;
		IndexBuffer(const IndexBuffer&&) = delete;
		IndexBuffer& operator=(const IndexBuffer&) = delete;
		const IndexBuffer& operator=(const IndexBuffer&&) = delete;
		~IndexBuffer();

		void BufferData(const VertexArray& vao,
			uint32_t* data,
			size_t size,
			BufferMode mode = STATIC) const;

		IndexBufferHandle getHandle() const;
		void release();
	private:
		IndexBufferHandle index_buffer_handle;
	};

	typedef unsigned int VertexBufferHandle;

	class VertexBuffer
	{
	public:
		enum BufferMode
		{
			STREAM,
			STATIC,
			DYNAMIC
		};

		VertexBuffer();
		VertexBuffer(VertexArray& vao, const Vertex* data, size_t size, BufferMode mode = STATIC);
		VertexBuffer(const VertexBuffer&) noexcept;
		VertexBuffer(VertexBuffer&&) noexcept;
		VertexBuffer& operator=(const VertexBuffer&) noexcept;
		const VertexBuffer& operator=(VertexBuffer&&) noexcept;
		~VertexBuffer();

		void BufferData(const VertexArray& vao,
			const Vertex* data,
			size_t size,
			BufferMode mode = STATIC) const;

		VertexBufferHandle getHandle() const;
		void release();
	protected:
		VertexBufferHandle vertex_buffer_handle;
	};

	//////////////////////////////////////////////////
}
//////////////////////////////////////////////////
#endif
//////////////////////////////////////////////////