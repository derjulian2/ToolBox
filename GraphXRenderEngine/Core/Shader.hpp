//////////////////////////////////////////////////
#ifndef GRX_SHADER_H
#define GRX_SHADER_H
//////////////////////////////////////////////////
#include <glad/glad.h>
#define UTIL_MOD_FILEMANIP
#include "../Dependencies/utilitylib.hpp"
#include <initializer_list>
#include <glm/glm.hpp>
//////////////////////////////////////////////////
namespace grx
{
	//////////////////////////////////////////////////

	class Shader
	{
	public:
		enum ShaderType
		{
			VERTEX_SHADER,
			FRAGMENT_SHADER
		};

		Shader(const std::filesystem::path& path, ShaderType type);
		Shader(const Shader&) = delete;
		Shader(const Shader&&) = delete;
		Shader& operator=(const Shader&) = delete;
		const Shader& operator=(const Shader&&) = delete;

		unsigned int getHandle() const;
		ShaderType getType() const;
	private:
		unsigned int shader_handle;
		ShaderType shader_type;
	};

	class ShaderProgram
	{
	public:
		ShaderProgram(const std::initializer_list<std::reference_wrapper<Shader>>&);
		ShaderProgram(const ShaderProgram&) = delete;
		ShaderProgram(const ShaderProgram&&) = delete;
		ShaderProgram& operator=(const ShaderProgram&) = delete;
		const ShaderProgram& operator=(const ShaderProgram&&) = delete;

		void use() const;
		void setUniform(const std::string& uniform_name, glm::mat4 matrix) const;
		void setUniform(const std::string& uniform_name, unsigned int i) const;

		unsigned int getHandle() const;
	private:
		unsigned int program_handle;
	};

	//////////////////////////////////////////////////
}
//////////////////////////////////////////////////
#endif
//////////////////////////////////////////////////