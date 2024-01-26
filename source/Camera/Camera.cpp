#include "Camera.h"
Camera::Camera(glm::vec3 camera_position, glm::vec3 camera_up)
{
    Position = camera_position;
    Up = camera_up;
}
glm::mat4 Camera::GetView()
{
    return glm::lookAt(Position, Position + Front, Up);
}

void Camera::processmovement(Camera_Movement direction, float deltatime)
{

    float velocity = MovementSpeed * deltatime;
    if (direction == FORWARD)
    {
        Position += Front * velocity;
    }
    if (direction == BACKWARD)
    {
        Position -= Front * velocity;
    }
    if (direction == LEFT)
    {
        Right = glm::normalize(glm::cross(Front, Up));

        Position -= Right * velocity;
    }
    if (direction == RIGHT)
    {
        Right = glm::normalize(glm::cross(Front, Up));

        Position += Right * velocity;
    }

}

void Camera::processmouseinput(float xoffset, float yoffset)
{
    xoffset *= mousesensitivity;
    yoffset *= mousesensitivity;
    yaw += xoffset;
    pitch += yoffset;
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;
    updatevectors();
}

void Camera::updatevectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    Front = glm::normalize(front);
}