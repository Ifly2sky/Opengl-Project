#pragma once
#include <GLEW/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};




class Camera
{
public:
    float MovementSpeed = 5.0f;
    float mousesensitivity = 0.1f;


    Camera(glm::vec3 camera_position, glm::vec3 camera_up);
    glm::mat4 GetView();
    void processmovement(Camera_Movement direction, float deltatime);
    void processmouseinput(float xoffset, float yoffset);

private:
    void updatevectors();
    glm::vec3 Position = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 Front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 Right;
    float yaw = 0.0f;
    float pitch = 0.0f;
};

