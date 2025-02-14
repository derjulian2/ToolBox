//////////////////////////////////////////////////
#include "Shader.hpp"
//////////////////////////////////////////////////
grx::Shader::Shader(const std::filesystem::path& path, ShaderType type)
{
	std::string shader_source;
	if (!utility::readfile(path, shader_source))
	{
		throw std::runtime_error("could not read shader-source");
	}
	const char* source_cstr = shader_source.c_str();
	
	unsigned int glshader_type;
	shader_type = type;
	switch (type)
	{
	case (VERTEX_SHADER):
		glshader_type = GL_VERTEX_SHADER;
		break;
	case (FRAGMENT_SHADER):
		glshader_type = GL_FRAGMENT_SHADER;
		break;
	}
	
	shader_handle = glCreateShader(glshader_type);
	glShaderSource(shader_handle, 1, &source_cstr, NULL);

	glCompileShader(shader_handle);
	int success;
	glGetShaderiv(shader_handle, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		throw std::runtime_error("shader compilation failed");
	}
}
unsigned int grx::Shader::getHandle() const
{
	return shader_handle;
}
grx::Shader::ShaderType grx::Shader::getType() const
{
	return shader_type;
}
//////////////////////////////////////////////////
grx::ShaderProgram::ShaderProgram(const std::initializer_list<std::reference_wrapper<Shader>>& shaders)
{
	program_handle = glCreateProgram();
	for (const Shader& shader : shaders)
	{
		glAttachShader(program_handle, shader.getHandle());
	}
	glLinkProgram(program_handle);
	int success;
	glGetProgramiv(program_handle, GL_LINK_STATUS, &success);
	if (!success)
	{
		throw std::runtime_error("shader program linking failed");
	}
	for (const Shader& shader : shaders)
	{
		glDeleteShader(shader.getHandle());
	}
}
void grx::ShaderProgram::use() const
{
	glUseProgram(program_handle);
}
unsigned int grx::ShaderProgram::getHandle() const
{
	return program_handle;
}
void grx::ShaderProgram::setUniform(const std::string& uniform_name, glm::mat4 matrix) const
{
	int uniform_location = glGetUniformLocation(program_handle, uniform_name.c_str());
	if (uniform_location == -1)
		throw std::runtime_error("uniform with name '" + uniform_name + "' could not be located in shader program");
	glUniformMatrix4fv(uniform_location, 1, GL_FALSE, &matrix[0][0]);
}
void grx::ShaderProgram::setUniform(const std::string& uniform_name, unsigned int i) const
{
	int uniform_location = glGetUniformLocation(program_handle, uniform_name.c_str());
	if (uniform_location == -1)
		throw std::runtime_error("uniform with name '" + uniform_name + "' could not be located in shader program");
	glUniform1i(uniform_location, i);
}
//////////////////////////////////////////////////