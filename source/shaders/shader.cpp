#include "shader.h"
#include <GLEW/glew.h>

#include <GLFW/glfw3.h>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

Shader::Shader(const std::string vertexshadersource, const std::string fragmentshadersource)
{
	//VertexShader
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const char* vertexshadersourcecharp = vertexshadersource.c_str();

	glShaderSource(vertexShader, 1, &vertexshadersourcecharp, NULL);
	glCompileShader(vertexShader);

	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "Error compiling vertex shader: " << infoLog << std::endl;
	}


	//FragmentShader
	unsigned int fragmentShader = glCreateShader(GLenum(GL_FRAGMENT_SHADER));
	const char* fragmenshadersourcecharp = fragmentshadersource.c_str();

	glShaderSource(fragmentShader, 1, &fragmenshadersourcecharp, NULL);
	glCompileShader(fragmentShader);

	int success2;
	char infolog2[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success2);
	if (!success2)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infolog2);
		std::cout << "Error compiling Fragment shader: " << infolog2 << std::endl;
	}

	//ShaderProgram
	Handle = glCreateProgram();

	glAttachShader(Handle, vertexShader);
	glAttachShader(Handle, fragmentShader);
	glLinkProgram(Handle);

	//deletes vertex and fragment shader
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	int success3;
	char infolog3[512];
	glGetProgramiv(Handle, GL_LINK_STATUS, &success3);
	if (!success3) {
		glGetProgramInfoLog(Handle, 512, NULL, infolog3);
		std::cout << "Error linking Shader program: " << infolog3 << std::endl;

	}
}


void Shader::Use()
{
	try
	{
		glUseProgram(this->Handle);
	}
	catch (...)
	{
		std::cout << "error using shader program\n";
	}
}


void Shader::SetBool(std::string& name, bool value)
{
	try
	{
		glUniform1i(glGetUniformLocation(Handle, name.c_str()), (int)value);


	}
	catch (...)
	{
		std::cout << "error setting bool  (" << name.c_str() << ")\n";
	}
}
void Shader::SetFloat(const char* name, float value)
{
	try
	{
		glUniform1f(glGetUniformLocation(Handle, name), value);

	}
	catch (...)
	{
		std::cout << "error setting float  (" << name << ")\n";
	}
}
void Shader::SetInt(std::string name, int value)
{
	try
	{
		glUniform1i(glGetUniformLocation(Handle, name.c_str()), value);
	}
	catch (...)
	{
		std::cout << "error setting int  (" << name.c_str() << ")\n";
	}
}
void Shader::SetVec3(std::string name, glm::vec3 value)
{
	try 
	{
		glUniform3fv(glGetUniformLocation(Handle, name.c_str()), 1, glm::value_ptr(value));
	}
	catch (...) 
	{
		std::cout << "error setting vec3 (" << name.c_str() << ")\n";
	}
}
void Shader::SetMat4(std::string name, glm::mat4 value)
{
	try
	{
		glUniformMatrix4fv(glGetUniformLocation(Handle, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
	}
	catch (...)
	{
		std::cout << "error setting mat4  (" << name.c_str() << ")\n";
	}
}
void Shader::SetVec4(const char* name, glm::vec4 value)
{
	try
	{
		glUniform4fv(glGetUniformLocation(Handle, name), 1, glm::value_ptr(value));
	}
	catch (...)
	{
		std::cout << "error setting vec3 (" << name << ")\n";
	}
}