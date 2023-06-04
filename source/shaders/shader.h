#pragma once
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class Shader
{
private:
public:
    unsigned int Handle;
    Shader(const std::string vertexshadersource, const std::string fragmentshadersource);
    void Use();
    void SetInt(std::string name, int value);
    void SetBool(std::string& name, bool value);
    void SetFloat(std::string& name, float value);
    void SetMat4(std::string name, glm::mat4 value);
};
