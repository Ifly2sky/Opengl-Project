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
//Shader* shader;
Shader* coloredCubeShader;
Shader* lightShader;
Camera* camera = new Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
glm::vec3 cubePos(0.0f, 0.0f, 0.0f);

int width = 1000, height = 800;
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
template<class Destination, class Copyables, class... type>
Destination PatchVertices(Destination destination, Copyables first, Copyables second, type ... rest);

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
    //shader = new Shader(file.ReadAllText("shader.vert"), file.ReadAllText("shader.frag"));
    coloredCubeShader = new Shader(file.ReadAllText("coloredCubeShader.vert"), file.ReadAllText("coloredCubeshader.frag"));
    lightShader = new Shader(file.ReadAllText("lightShader.vert"), file.ReadAllText("lightShader.frag"));

    /*
        lightingShader.use();
        lightingShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);*/

    /*float vertices[] = {
        0.5f,  0.5f, 0.0f, 1.0f, 1.0f, // top right
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f, 0.0f, 1.0f // top left
    };*/

    float lightVertices[] = {
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
    };
    float cubeVertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    /*Vertex v1{};
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

    static std::array<Vertex, 4> vertices2 = { v1, v2, v3, v4 };*/

    //unsigned int indices[] = {  // note that we start from 0!
    //    0, 1, 3,   // first triangle
    //    1, 2, 3    // second triangle
    //};

    //shader->Use();
    //coloredCubeShader->Use();

    VertexArrayObject CubeVAO = VertexArrayObject();
    VertexBufferObject CubeVBO = VertexBufferObject(cubeVertices, sizeof(cubeVertices), { 3, 3 }, 6 * sizeof(float), { 0, 3 * sizeof(float) });
    VertexArrayObject LightVAO = VertexArrayObject();
    VertexBufferObject LightVBO = VertexBufferObject(lightVertices, sizeof(lightVertices), { 3 }, 3 * sizeof(float), { 0 });

    //ElementBufferObject EBO = ElementBufferObject(indices, sizeof(indices));
    //Vertex finalVertices[4];
    //PatchVertices(finalVertices, vertices2.data(), vertices2.data()+ vertices2.size());
    //std::copy(vertices2.data(), vertices2.data() + vertices2.size(), finalVertices);
    //garbage time -----------------------------------------------------------------------------------------------------------------------

    /*unsigned int texture;
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
    stbi_image_free(data);*/

    //garbage time end ------------------------------------------------------------------------------------------------------------------
    

    coloredCubeShader->Use();
    coloredCubeShader->SetVec3("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
    coloredCubeShader->SetVec3("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
    coloredCubeShader->SetVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
    coloredCubeShader->SetFloat("material.shininess", 32.0f);
    coloredCubeShader->SetVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
    coloredCubeShader->SetVec3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
    coloredCubeShader->SetVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        //get distance from lcube to light
        glm::vec3 dist = cubePos - lightPos;
        //calculate rotation of 90 degrees
        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 1.0f));
        glm::vec4 result = trans * glm::vec4(dist, 1.0f);
        //apply move object
        lightPos += glm::vec3(result.x, result.y, result.z) * 0.5f * deltaTime;

        CheckKeys();
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        coloredCubeShader->Use();
        coloredCubeShader->SetVec3("viewPos", camera->Position);
        coloredCubeShader->SetVec3("lightPos", lightPos);

        glm::mat4 model = glm::mat4(1.0);
        model = glm::translate(model, cubePos);
        coloredCubeShader->SetMat4("model", model);

        auto view = camera->GetView();
        coloredCubeShader->SetMat4("view", view);

        auto projection = glm::perspective(glm::radians(fov), (float)width / (float)height, 0.0001f, 1000000.0f);
        coloredCubeShader->SetMat4("projection", projection);

        CubeVAO.Use();//glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        lightShader->Use();
        lightShader->SetMat4("projection", projection);
        lightShader->SetMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));
        lightShader->SetMat4("model", model);

        LightVAO.Use();//glBindVertexArray(lightCubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //glBindTexture(GL_TEXTURE_2D, texture);

        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_POLYGON_OFFSET_LINE);
    glPolygonOffset(-1, -1);
    glDepthFunc(GL_LESS);
    glDepthRange(0.0f, 1.0f);

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
template<class Destination, class Copyables, class... type>
Destination PatchVertices(Destination destination, Copyables first, Copyables second, type ... rest) {
    if (first == NULL) {
        return destination;
    }
    std::copy(first, second, destination);
    return destination;
}