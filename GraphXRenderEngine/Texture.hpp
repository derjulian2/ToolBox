//////////////////////////////////////////////////
#ifndef GRX_TEXTURE_H
#define GRX_TEXTURE_H
//////////////////////////////////////////////////
#include <glad/glad.h>
#include <filesystem>
#include <stdexcept>
//////////////////////////////////////////////////
namespace grx
{
	//////////////////////////////////////////////////

	typedef unsigned int TextureHandle;

	class Texture
	{
	public:
		Texture(const std::filesystem::path& path);
		Texture(const Texture&) = delete;
		Texture(const Texture&&) = delete;
		Texture& operator=(const Texture&) = delete;
		const Texture& operator=(const Texture&&) = delete;
		~Texture();

		TextureHandle getHandle() const;

		void release();
	private:
		int width;
		int height;
		int nrChannels;
		TextureHandle texture_handle;
	};

	//////////////////////////////////////////////////
}
//////////////////////////////////////////////////
#endif
//////////////////////////////////////////////////