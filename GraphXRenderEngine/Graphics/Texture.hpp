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
		enum TextureType
		{
			SPECULAR,
			DIFFUSE
		};

		Texture(const std::filesystem::path& path, TextureType type = DIFFUSE);
		Texture(const Texture&) = delete;
		Texture(Texture&&) noexcept;
		Texture& operator=(const Texture&) = delete;
		const Texture& operator=(Texture&&) noexcept;
		~Texture();

		TextureHandle getHandle() const;
		TextureType getType() const;

		void release();
	private:
		int width;
		int height;
		int nrChannels;
		TextureHandle texture_handle;
		TextureType type;
	};

	//////////////////////////////////////////////////
}
//////////////////////////////////////////////////
#endif
//////////////////////////////////////////////////