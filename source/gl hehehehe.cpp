// gl hehehehe.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <FileHandler.h>
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <shaders/shader.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <objects/VertexArrayObject.h>
#include <objects/VertexBufferObject.h>
#include <objects/ElementBufferObject.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Camera/Camera.h>
#include <thread>
#include <chrono>
#include <objects/VertexData.h>

using namespace std::chrono_literals;

FileHandler file;
Shader* shader;
Camera* camera = new Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

int width = 400, height = 400;
bool isFullScreen = false;
const int refreshrate = 100;
float  deltaTime, lastFrame;

GLFWwindow* window;

bool firstMouse = true;
int lastX, lastY;
float fov = 90;

int CheckKeys(int numero = 0);
void OnLoad();
void Init();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main()
{
    if (glfwInit()) {
        glfwWindowHint(GLFW_DEPTH_BITS, 24);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    }

    window = glfwCreateWindow(width, height, "yay", nullptr, nullptr);

    glfwMakeContextCurrent(window);

    OnLoad();

    /*std::cout << file.ReadAllText("shader.vert") << "\n";
    std::cout << file.ReadAllText("shader.frag") << "\n";*/
    shader = new Shader(file.ReadAllText("shader.vert"), file.ReadAllText("shader.frag"));

    /*float vertices[] = {
        0.5f,  0.5f, 0.0f, 1.0f, 1.0f, // top right
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f, 0.0f, 1.0f // top left
    };*/
    Vertex v1{};
    v1.Position = { 0.5f, 0.5f, 0.0f };
    v1.TexCoordinates = { 1.0f, 1.0f };
    Vertex v2{};
    v2.Position = { 0.5f, -0.5f, 0.0f };
    v2.TexCoordinates = { 1.0f, 0.0f };
    Vertex v3{};
    v3.Position = { -0.5f, -0.5f, 0.0f };
    v3.TexCoordinates = { 0.0f, 0.0f };
    Vertex v4{};
    v4.Position = { -0.5f,  0.5f, 0.0f };
    v4.TexCoordinates = { 0.0f, 1.0f };

    static std::array<Vertex, 4> vertices2 = { v1, v2, v3, v4 };

    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };

    shader->Use();
    VertexArrayObject VAO = VertexArrayObject();
    VertexBufferObject VBO = VertexBufferObject(vertices2.data());
    ElementBufferObject EBO = ElementBufferObject(indices, sizeof(indices));

    Vertex finalVertices[4];

    std::copy(vertices2.data(), vertices2.data() + vertices2.size(), finalVertices);

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(finalVertices), finalVertices);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //garbage time -----------------------------------------------------------------------------------------------------------------------

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("Stone.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    //garbage time end ------------------------------------------------------------------------------------------------------------------

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        CheckKeys();

        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(sin(glfwGetTime()), tan(glfwGetTime()), cos(glfwGetTime()), 1);

        glm::mat4 model = glm::mat4(1.0);
        shader->SetMat4("model", model);

        auto view = camera->GetView();
        shader->SetMat4("view", view);

        auto projection = glm::perspective(glm::radians(fov), (float)width / (float)height, 0.001f, 1000000.0f);
        shader->SetMat4("projection", projection);

        shader->Use();
        VAO.Use();
        VBO.Use();
        EBO.Use();

        glBindTexture(GL_TEXTURE_2D, texture);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


        glfwSwapBuffers(window);

        glfwPollEvents();
    }
    glfwTerminate();
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastY = ypos;
        lastX = xpos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera->processmouseinput(xoffset, yoffset);

}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{

    fov -= yoffset;
    if (fov > 179)
        fov = 179;
    if (fov < 1)
        fov = 1;
    std::cout << "fov: " << fov << std::endl;
}
void OnLoad() 
{
    Init();
    //glEnable(GL_DEPTH_TEST);
    //glEnable(GL_POLYGON_OFFSET_LINE);
    //glPolygonOffset(-1, -1);
    //glDepthFunc(GL_LESS);
    //glDepthRange(0.0f, 1.0f);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

}
void Init()
{
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cout << "glewInit failed: " << glewGetErrorString(err) << std::endl;
        exit(1);
    }
}
int CheckKeys(int numero)
{

    if (glfwGetKey(window, GLFW_KEY_F))
    {
        if (isFullScreen)
        {
            std::this_thread::sleep_for(100ms);
            //const GLFWvidmode* mode = glfwGetVideoMode(_window->monitor);
            isFullScreen = false;
            std::cout << "changed to windowed mode\n";
            glfwSetWindowMonitor(window, nullptr, 0, 30, width, height - 30, refreshrate);

        }
        else
        {
            glfwSetWindowMonitor(window, NULL, 0, 0, width, height, refreshrate);
            std::cout << "changed to fullscreen mode\n";
            isFullScreen = true;
        }
        std::this_thread::sleep_for(200ms);
    }
    if (glfwGetKey(window, GLFW_KEY_ESCAPE))
    {
        std::cout << "trying to close window...";
        //OnUnload();
        return 1;
    }


    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera->processmovement(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera->processmovement(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera->processmovement(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera->processmovement(RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
    {
        //togglewireframe();

    }


    return 0;

}