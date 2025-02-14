//////////////////////////////////////////////////
#include "Drawables.hpp"
//////////////////////////////////////////////////
grx::TriangleShape::TriangleShape(Vertex v1, Vertex v2, Vertex v3)
{
	setVertices(v1, v2, v3);
}
void grx::TriangleShape::setVertices(Vertex v1, Vertex v2, Vertex v3)
{
	vertices[0] = v1;
	vertices[1] = v2;
	vertices[2] = v3;
	buffer.BufferData(vertex_array, vertices.data(), 3);
}
void grx::TriangleShape::draw() const
{
	glBindVertexArray(vertex_array.getHandle());
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
}
//////////////////////////////////////////////////
grx::CubeShape::CubeShape(glm::vec3 position, float size, Color color)
{
	setPos(position);
	setSize(size);
	setColor(color);
}
glm::vec3 grx::CubeShape::getPos() const
{
	return pos;
}
float grx::CubeShape::getSize() const
{
	return size;
}
grx::Color grx::CubeShape::getColor() const
{
	return color;
}
void grx::CubeShape::setPos(glm::vec3 pos)
{
	this->pos = pos;
	setVertices();
}
void grx::CubeShape::setSize(float size)
{
	this->size = size;
	setVertices();
}
void grx::CubeShape::setColor(Color color)
{
	this->color = color;
	setVertices();
}
void grx::CubeShape::setTexture(Texture& tex)
{
	texture = tex;
}
void grx::CubeShape::draw() const
{
	glBindVertexArray(vertex_array.getHandle());
	if (texture.has_value())
	{
		if (texture->get().getHandle())
		{
			glBindTexture(GL_TEXTURE_2D, texture->get().getHandle());
		}
	}

	glDrawArrays(GL_TRIANGLES, 0, 36);
	
	if (texture.has_value())
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	glBindVertexArray(0);
}
void grx::CubeShape::setVertices()
{
	// front face
	vertices[ 0] = Vertex(pos + glm::vec3(-size, -size, -size), color, glm::vec2(0, 0));
	vertices[ 1] = Vertex(pos + glm::vec3(+size, -size, -size), color, glm::vec2(1, 0));
	vertices[ 2] = Vertex(pos + glm::vec3(+size, +size, -size), color, glm::vec2(1, 1));
	vertices[ 3] = Vertex(pos + glm::vec3(-size, -size, -size), color, glm::vec2(0, 0));
	vertices[ 4] = Vertex(pos + glm::vec3(-size, +size, -size), color, glm::vec2(0, 1));
	vertices[ 5] = Vertex(pos + glm::vec3(+size, +size, -size), color, glm::vec2(1, 1));
	// top face
	vertices[ 6] = Vertex(pos + glm::vec3(-size, +size, -size), color, glm::vec2(0, 0));
	vertices[ 7] = Vertex(pos + glm::vec3(+size, +size, -size), color, glm::vec2(1, 0));
	vertices[ 8] = Vertex(pos + glm::vec3(+size, +size, +size), color, glm::vec2(1, 1));
	vertices[ 9] = Vertex(pos + glm::vec3(-size, +size, -size), color, glm::vec2(0, 0));
	vertices[10] = Vertex(pos + glm::vec3(-size, +size, +size), color, glm::vec2(0, 1));
	vertices[11] = Vertex(pos + glm::vec3(+size, +size, +size), color, glm::vec2(1, 1));
	// back face
	vertices[12] = Vertex(pos + glm::vec3(-size, +size, +size), color, glm::vec2(0, 0));
	vertices[13] = Vertex(pos + glm::vec3(+size, +size, +size), color, glm::vec2(1, 0));
	vertices[14] = Vertex(pos + glm::vec3(+size, -size, +size), color, glm::vec2(1, 1));
	vertices[15] = Vertex(pos + glm::vec3(-size, +size, +size), color, glm::vec2(0, 0));
	vertices[16] = Vertex(pos + glm::vec3(-size, -size, +size), color, glm::vec2(0, 1));
	vertices[17] = Vertex(pos + glm::vec3(+size, -size, +size), color, glm::vec2(1, 1));
	// bottom face
	vertices[18] = Vertex(pos + glm::vec3(-size, -size, +size), color, glm::vec2(0, 0));
	vertices[19] = Vertex(pos + glm::vec3(+size, -size, +size), color, glm::vec2(1, 0));
	vertices[20] = Vertex(pos + glm::vec3(+size, -size, -size), color, glm::vec2(1, 1));
	vertices[21] = Vertex(pos + glm::vec3(-size, -size, +size), color, glm::vec2(0, 0));
	vertices[22] = Vertex(pos + glm::vec3(-size, -size, -size), color, glm::vec2(0, 1));
	vertices[23] = Vertex(pos + glm::vec3(+size, -size, -size), color, glm::vec2(1, 1));
	// left face
	vertices[24] = Vertex(pos + glm::vec3(-size, -size, +size), color, glm::vec2(0, 0));
	vertices[25] = Vertex(pos + glm::vec3(-size, -size, -size), color, glm::vec2(1, 0));
	vertices[26] = Vertex(pos + glm::vec3(-size, +size, -size), color, glm::vec2(1, 1));
	vertices[27] = Vertex(pos + glm::vec3(-size, -size, +size), color, glm::vec2(0, 0));
	vertices[28] = Vertex(pos + glm::vec3(-size, +size, +size), color, glm::vec2(0, 1));
	vertices[29] = Vertex(pos + glm::vec3(-size, +size, -size), color, glm::vec2(1, 1));
	// right face
	vertices[30] = Vertex(pos + glm::vec3(+size, -size, -size), color, glm::vec2(0, 0));
	vertices[31] = Vertex(pos + glm::vec3(+size, -size, +size), color, glm::vec2(1, 0));
	vertices[32] = Vertex(pos + glm::vec3(+size, +size, +size), color, glm::vec2(1, 1));
	vertices[33] = Vertex(pos + glm::vec3(+size, -size, -size), color, glm::vec2(0, 0));
	vertices[34] = Vertex(pos + glm::vec3(+size, +size, -size), color, glm::vec2(0, 1));
	vertices[35] = Vertex(pos + glm::vec3(+size, +size, +size), color, glm::vec2(1, 1));
	vertex_buffer.BufferData(vertex_array, vertices.data(), 36);
}
//////////////////////////////////////////////////