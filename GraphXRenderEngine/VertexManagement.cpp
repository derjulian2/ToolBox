//////////////////////////////////////////////////
#include "VertexManagment.hpp"
//////////////////////////////////////////////////
grx::Color::Color(glm::vec4 rgba)
{
	r = rgba.x / 255.0f;
	g = rgba.y / 255.0f;
	b = rgba.z / 255.0f;
	a = rgba.w / 255.0f;
}
grx::Color::Color(float r, float g, float b, float a)
	: Color(glm::vec4(r, g, b, a))
{

}
//////////////////////////////////////////////////
grx::Vertex::Vertex(glm::vec3 pos, Color color, glm::vec2 texcoords)
	: pos(pos), color(color), texcoords(texcoords)
{

}
//////////////////////////////////////////////////
grx::VertexArray::VertexArray()
{
	glGenVertexArrays(1, &vertex_array_handle);
}
grx::VertexArray::~VertexArray()
{
	release();
}
grx::VertexArrayHandle grx::VertexArray::getHandle() const
{
	return vertex_array_handle;
}
void grx::VertexArray::release()
{
	glDeleteVertexArrays(1, &vertex_array_handle);
	vertex_array_handle = 0;
}
//////////////////////////////////////////////////
grx::IndexBuffer::IndexBuffer()
{
	glGenBuffers(1, &index_buffer_handle);
}
grx::IndexBuffer::IndexBuffer(const VertexArray& vao,
	uint32_t* data,
	size_t size,
	BufferMode mode)
{
	glGenBuffers(1, &index_buffer_handle);
	BufferData(vao, data, size, mode);
}
grx::IndexBuffer::~IndexBuffer()
{
	release();
}
void grx::IndexBuffer::BufferData(const VertexArray& vao,
	uint32_t* data,
	size_t size,
	BufferMode mode) const
{
	if (!index_buffer_handle)
		throw std::runtime_error("vertex buffer handle was invalid");
	if (!vao.getHandle())
		throw std::runtime_error("vertex array handle was invalid");

	unsigned int glmode;
	switch (mode)
	{
	case (STREAM):
		glmode = GL_STREAM_DRAW;
		break;
	case (STATIC):
		glmode = GL_STATIC_DRAW;
		break;
	case (DYNAMIC):
		glmode = GL_DYNAMIC_DRAW;
		break;
	}

	glBindVertexArray(vao.getHandle());

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_handle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * size, data, glmode);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
grx::IndexBufferHandle grx::IndexBuffer::getHandle() const
{
	return index_buffer_handle;
}
void grx::IndexBuffer::release()
{
	glDeleteBuffers(1, &index_buffer_handle);
	index_buffer_handle = 0;
}
//////////////////////////////////////////////////
grx::VertexBuffer::VertexBuffer()
{
	glGenBuffers(1, &vertex_buffer_handle);
}
grx::VertexBuffer::VertexBuffer(VertexArray& vao, const Vertex* data, size_t size, BufferMode mode)
{
	glGenBuffers(1, &vertex_buffer_handle);
	BufferData(vao, data, size, mode);
}
grx::VertexBuffer::VertexBuffer(const VertexBuffer&) noexcept
{
	if (vertex_buffer_handle)
	{
		release()
	}
	glGenBuffers(1, &vertex_buffer_handle);
}
grx::VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept
{
	if (vertex_buffer_handle)
	{
		release()
	}
	this->vertex_buffer_handle = other.vertex_buffer_handle;
	other.vertex_buffer_handle = 0;
}
grx::VertexBuffer& grx::VertexBuffer::operator=(const VertexBuffer&) noexcept
{
	if (vertex_buffer_handle)
	{
		release()
	}
	glGenBuffers(1, &vertex_buffer_handle);
	return *this;
}
const grx::VertexBuffer& grx::VertexBuffer::operator=(VertexBuffer&& other) noexcept
{
	if (vertex_buffer_handle)
	{
		release()
	}
	this->vertex_buffer_handle = other.vertex_buffer_handle;
	other.vertex_buffer_handle = 0;
	return *this;
}
grx::VertexBuffer::~VertexBuffer()
{
	release();
}
void grx::VertexBuffer::BufferData(const VertexArray& vao, const Vertex* data, size_t size, BufferMode mode) const
{
	if (!vertex_buffer_handle)
		throw std::runtime_error("vertex buffer handle was invalid");
	if (!vao.getHandle())
		throw std::runtime_error("vertex array handle was invalid");

	unsigned int glmode;
	switch (mode)
	{
	case (STREAM):
		glmode = GL_STREAM_DRAW;
		break;
	case (STATIC):
		glmode = GL_STATIC_DRAW;
		break;
	case (DYNAMIC):
		glmode = GL_DYNAMIC_DRAW;
		break;
	}

	glBindVertexArray(vao.getHandle());

	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_handle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * size, data, glmode);

	// position data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);
	// color data
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// tex coord data
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(7 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}
grx::VertexBufferHandle grx::VertexBuffer::getHandle() const
{
	return vertex_buffer_handle;
}
void grx::VertexBuffer::release()
{
	glDeleteBuffers(1, &vertex_buffer_handle);
	vertex_buffer_handle = 0;
}
//////////////////////////////////////////////////
