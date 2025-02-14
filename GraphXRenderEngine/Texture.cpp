//////////////////////////////////////////////////
#include "Texture.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
//////////////////////////////////////////////////
grx::Texture::Texture(const std::filesystem::path& path)
{
	unsigned char* data = stbi_load(path.string().c_str(), &width, &height, &nrChannels, 0);
	if (!data)
	{
		throw std::runtime_error("could not load texture at " + path.string());
	}
	glGenTextures(1, &texture_handle);
	glBindTexture(GL_TEXTURE_2D, texture_handle);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data);
}
grx::Texture::~Texture()
{
	release();
}
grx::TextureHandle grx::Texture::getHandle() const
{
	return texture_handle;
}
void grx::Texture::release()
{
	glDeleteTextures(1, &texture_handle);
	texture_handle = 0;
}
//////////////////////////////////////////////////