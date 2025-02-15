//////////////////////////////////////////////////
#include "Drawables.hpp"
//////////////////////////////////////////////////
void grx::Transformable::setPos(glm::vec3 pos)
{
	this->position = pos;
}
void grx::Transformable::setScale(glm::vec3 scale)
{
	this->scale = scale;
}
void grx::Transformable::setRotation(glm::vec3 rotation)
{
	this->rotation = rotation;
}
glm::vec3 grx::Transformable::getPos() const
{
	return position;
}
glm::vec3 grx::Transformable::getScale() const
{
	return scale;
}
glm::vec3 grx::Transformable::getRotation() const
{
	return rotation;
}
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
	setScale(glm::vec3(size,size,size));
	setColor(color);
	uint32_t indices[36] =
	{
		 0, 1, 2, 3, 4, 5, 
		 6, 7, 8, 9,10,11,
		12,13,14,15,16,17,
		18,19,20,21,22,23,
		24,25,26,27,28,29,
		30,31,32,33,34,35
	};
	index_buffer.BufferData(vertex_array, indices, 36);
	update();
}
grx::Color grx::CubeShape::getColor() const
{
	return color;
}
void grx::CubeShape::setColor(Color color)
{
	this->color = color;
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
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer.getHandle());
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	if (texture.has_value())
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	glBindVertexArray(0);
}
void grx::CubeShape::update()
{
	// front face
	vertices[0] = Vertex(glm::vec3(-1, -1, -1), color, glm::vec2(0, 0));
	vertices[1] = Vertex(glm::vec3(+1, -1, -1), color, glm::vec2(1, 0));
	vertices[2] = Vertex(glm::vec3(+1, +1, -1), color, glm::vec2(1, 1));
	vertices[3] = Vertex(glm::vec3(-1, -1, -1), color, glm::vec2(0, 0));
	vertices[4] = Vertex(glm::vec3(-1, +1, -1), color, glm::vec2(0, 1));
	vertices[5] = Vertex(glm::vec3(+1, +1, -1), color, glm::vec2(1, 1));
	// top face
	vertices[6] = Vertex(glm::vec3(-1, +1, -1), color, glm::vec2(0, 0));
	vertices[7] = Vertex(glm::vec3(+1, +1, -1), color, glm::vec2(1, 0));
	vertices[8] = Vertex(glm::vec3(+1, +1, +1), color, glm::vec2(1, 1));
	vertices[9] = Vertex(glm::vec3(-1, +1, -1), color, glm::vec2(0, 0));
	vertices[10] = Vertex(glm::vec3(-1, +1, +1), color, glm::vec2(0, 1));
	vertices[11] = Vertex(glm::vec3(+1, +1, +1), color, glm::vec2(1, 1));
	// back face
	vertices[12] = Vertex(glm::vec3(-1, +1, +1), color, glm::vec2(0, 0));
	vertices[13] = Vertex(glm::vec3(+1, +1, +1), color, glm::vec2(1, 0));
	vertices[14] = Vertex(glm::vec3(+1, -1, +1), color, glm::vec2(1, 1));
	vertices[15] = Vertex(glm::vec3(-1, +1, +1), color, glm::vec2(0, 0));
	vertices[16] = Vertex(glm::vec3(-1, -1, +1), color, glm::vec2(0, 1));
	vertices[17] = Vertex(glm::vec3(+1, -1, +1), color, glm::vec2(1, 1));
	// bottom face
	vertices[18] = Vertex(glm::vec3(-1, -1, +1), color, glm::vec2(0, 0));
	vertices[19] = Vertex(glm::vec3(+1, -1, +1), color, glm::vec2(1, 0));
	vertices[20] = Vertex(glm::vec3(+1, -1, -1), color, glm::vec2(1, 1));
	vertices[21] = Vertex(glm::vec3(-1, -1, +1), color, glm::vec2(0, 0));
	vertices[22] = Vertex(glm::vec3(-1, -1, -1), color, glm::vec2(0, 1));
	vertices[23] = Vertex(glm::vec3(+1, -1, -1), color, glm::vec2(1, 1));
	// left face
	vertices[24] = Vertex(glm::vec3(-1, -1, +1), color, glm::vec2(0, 0));
	vertices[25] = Vertex(glm::vec3(-1, -1, -1), color, glm::vec2(1, 0));
	vertices[26] = Vertex(glm::vec3(-1, +1, -1), color, glm::vec2(1, 1));
	vertices[27] = Vertex(glm::vec3(-1, -1, +1), color, glm::vec2(0, 0));
	vertices[28] = Vertex(glm::vec3(-1, +1, +1), color, glm::vec2(0, 1));
	vertices[29] = Vertex(glm::vec3(-1, +1, -1), color, glm::vec2(1, 1));
	// right face
	vertices[30] = Vertex(glm::vec3(+1, -1, -1), color, glm::vec2(0, 0));
	vertices[31] = Vertex(glm::vec3(+1, -1, +1), color, glm::vec2(1, 0));
	vertices[32] = Vertex(glm::vec3(+1, +1, +1), color, glm::vec2(1, 1));
	vertices[33] = Vertex(glm::vec3(+1, -1, -1), color, glm::vec2(0, 0));
	vertices[34] = Vertex(glm::vec3(+1, +1, -1), color, glm::vec2(0, 1));
	vertices[35] = Vertex(glm::vec3(+1, +1, +1), color, glm::vec2(1, 1));
	vertex_buffer.BufferData(vertex_array, vertices.data(), 36);
}
//////////////////////////////////////////////////
grx::Mesh::Mesh(const std::vector<Vertex>& vertices,
	const std::vector<uint32_t>& indices)
{
	this->vertices = vertices;
	this->indices = indices;
	setBuffers();
}
void grx::Mesh::setBuffers()
{
	vertex_buffer.BufferData(vertex_array, vertices.data(), vertices.size());
	index_buffer.BufferData(vertex_array, indices.data(), indices.size());
}
//////////////////////////////////////////////////
grx::Model::Model(const std::filesystem::path& path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path.string(), aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		throw std::runtime_error("error with loading model at '" + path.string() + "'");
	}

	processNode(scene->mRootNode, scene);
}
void grx::Model::setTexture(Texture& tex)
{
	texture = tex;
}
void grx::Model::draw() const
{
	if (texture.has_value())
	{
		if (texture->get().getHandle())
		{
			glBindTexture(GL_TEXTURE_2D,texture->get().getHandle());
		}
	}
	for (const Mesh& mesh : meshes)
	{
		glBindVertexArray(mesh.vertex_array.getHandle());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.index_buffer.getHandle());
		glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
	}
	if (texture.has_value())
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void grx::Model::processNode(aiNode* node, const aiScene* scene)
{
	for (size_t i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.emplace_back(toMesh(mesh, scene));
	}

	for (size_t i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

grx::Mesh grx::Model::toMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	for (size_t i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;

		vertex.pos = glm::vec3(
			mesh->mVertices[i].x,
			mesh->mVertices[i].y,
			mesh->mVertices[i].z
			);
		vertex.color = Color(255, 255, 255, 255);
		
		if (mesh->mTextureCoords[0])
		{
			vertex.texcoords = glm::vec2(
				mesh->mTextureCoords[0][i].x,
				mesh->mTextureCoords[0][i].y
			);
		}
		else
		{
			vertex.texcoords = glm::vec2(0.0f, 0.0f);
		}
		vertices.emplace_back(vertex);
	}

	for (size_t i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; j++)
		{
			indices.emplace_back(face.mIndices[j]);
		}
	}

	return Mesh(vertices, indices);
}
//////////////////////////////////////////////////