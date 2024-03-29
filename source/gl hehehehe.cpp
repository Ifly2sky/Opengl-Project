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

int width = 800, height = 800;
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
unsigned int LoadTexture(char const* path);

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
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };
    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };
    // positions of the point lights
    glm::vec3 pointLightPositions[] = {
        glm::vec3(0.7f,  0.2f,  2.0f),
        glm::vec3(2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3(0.0f,  0.0f, -3.0f)
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
    VertexBufferObject CubeVBO = VertexBufferObject(cubeVertices, sizeof(cubeVertices), { 3, 3, 2 }, 8 * sizeof(float), { 0, 3 * sizeof(float), 6 * sizeof(float)});
    VertexArrayObject LightVAO = VertexArrayObject();
    VertexBufferObject LightVBO = VertexBufferObject(lightVertices, sizeof(lightVertices), { 3 }, 3 * sizeof(float), { 0 });

    //ElementBufferObject EBO = ElementBufferObject(indices, sizeof(indices));
    //Vertex finalVertices[4];
    //PatchVertices(finalVertices, vertices2.data(), vertices2.data()+ vertices2.size());
    //std::copy(vertices2.data(), vertices2.data() + vertices2.size(), finalVertices);
    //garbage time -----------------------------------------------------------------------------------------------------------------------

    unsigned int diffuseMap = LoadTexture("container2.png");
    unsigned int specularMap = LoadTexture("container2_specular.png");

    //garbage time end ------------------------------------------------------------------------------------------------------------------
    

    coloredCubeShader->Use();
    coloredCubeShader->SetVec3("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
    coloredCubeShader->SetFloat("material.shininess", 64.0f);
    coloredCubeShader->SetInt("material.specular", 1);
    coloredCubeShader->SetInt("material.diffuse", 0);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        //get distance from cube to light
        glm::vec3 dist = cubePos - lightPos;
        //calculate rotation of 90 degrees
        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 1.0f));
        glm::vec4 result = trans * glm::vec4(dist, 1.0f);
        //apply move object
        lightPos += glm::vec3(result.x, result.y, result.z) * 0.5f * deltaTime;

        CheckKeys();
        glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        coloredCubeShader->Use();
        coloredCubeShader->SetVec3("viewPos", camera->Position);

        glm::vec3 lightColor;
        lightColor.x = 1.0f;//sin(glfwGetTime() * 2.0f);
        lightColor.y = 1.0f;//sin(glfwGetTime() * 0.7f);
        lightColor.z = 1.0f;//sin(glfwGetTime() * 1.3f);

        coloredCubeShader->SetVec3("spotLight.ambient", lightColor * 0.2f);
        coloredCubeShader->SetVec3("spotLight.diffuse", lightColor * 0.5f);
        coloredCubeShader->SetVec3("spotLight.specular", lightColor * 1.0f);
        coloredCubeShader->SetFloat("spotLight.constant", 1.0f);
        coloredCubeShader->SetFloat("spotLight.linear", 0.09f);
        coloredCubeShader->SetFloat("spotLight.quadratic", 0.032f);
        coloredCubeShader->SetVec3("spotLight.position", glm::vec3(camera->Position));
        coloredCubeShader->SetVec3("spotLight.direction", camera->Front);
        coloredCubeShader->SetFloat("spotLight.innerCutOff", glm::cos(glm::radians(25.0f)));
        coloredCubeShader->SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(35.0f)));

        coloredCubeShader->SetVec3("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
        coloredCubeShader->SetVec3("dirLight.ambient", lightColor * 0.05f);
        coloredCubeShader->SetVec3("dirLight.diffuse", lightColor * 0.4f);
        coloredCubeShader->SetVec3("dirLight.specular", lightColor * 0.5f);

        coloredCubeShader->SetVec3("pointLights[0].position", pointLightPositions[0]);
        coloredCubeShader->SetVec3("pointLights[0].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
        coloredCubeShader->SetVec3("pointLights[0].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
        coloredCubeShader->SetVec3("pointLights[0].specular", glm::vec3(1.0f, 1.0f, 1.0f));
        coloredCubeShader->SetFloat("pointLights[0].constant", 1.0f);
        coloredCubeShader->SetFloat("pointLights[0].linear", 0.09f);
        coloredCubeShader->SetFloat("pointLights[0].quadratic", 0.032f);

        coloredCubeShader->SetVec3("pointLights[1].position", pointLightPositions[1]);
        coloredCubeShader->SetVec3("pointLights[1].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
        coloredCubeShader->SetVec3("pointLights[1].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
        coloredCubeShader->SetVec3("pointLights[1].specular", glm::vec3(1.0f, 1.0f, 1.0f));
        coloredCubeShader->SetFloat("pointLights[1].constant", 1.0f);
        coloredCubeShader->SetFloat("pointLights[1].linear", 0.09f);
        coloredCubeShader->SetFloat("pointLights[1].quadratic", 0.032f);

        coloredCubeShader->SetVec3("pointLights[2].position", pointLightPositions[2]);
        coloredCubeShader->SetVec3("pointLights[2].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
        coloredCubeShader->SetVec3("pointLights[2].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
        coloredCubeShader->SetVec3("pointLights[2].specular", glm::vec3(1.0f, 1.0f, 1.0f));
        coloredCubeShader->SetFloat("pointLights[2].constant", 1.0f);
        coloredCubeShader->SetFloat("pointLights[2].linear", 0.09f);
        coloredCubeShader->SetFloat("pointLights[2].quadratic", 0.032f);

        coloredCubeShader->SetVec3("pointLights[3].position", pointLightPositions[3]);
        coloredCubeShader->SetVec3("pointLights[3].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
        coloredCubeShader->SetVec3("pointLights[3].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
        coloredCubeShader->SetVec3("pointLights[3].specular", glm::vec3(1.0f, 1.0f, 1.0f));
        coloredCubeShader->SetFloat("pointLights[3].constant", 1.0f);
        coloredCubeShader->SetFloat("pointLights[3].linear", 0.09f);
        coloredCubeShader->SetFloat("pointLights[3].quadratic", 0.032f);

        auto view = camera->GetView();
        coloredCubeShader->SetMat4("view", view);

        auto projection = glm::perspective(glm::radians(fov), (float)width / (float)height, 0.0001f, 1000000.0f);
        coloredCubeShader->SetMat4("projection", projection);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);

        CubeVAO.Use();//glBindVertexArray(cubeVAO);

        glm::mat4 model;
        for (unsigned int i = 0; i < 10; i++) {
            model = glm::mat4(1.0);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            coloredCubeShader->SetMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }


        lightShader->Use();
        lightShader->SetMat4("projection", projection);
        lightShader->SetMat4("view", view);
        lightShader->SetVec3("lightColor", lightColor);

        
        LightVAO.Use();//glBindVertexArray(lightCubeVAO);
        for (unsigned int i = 0; i < 4; i++) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f));
            lightShader->SetMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

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
unsigned int LoadTexture(char const* path) {
    unsigned int texture;
    glGenTextures(1, &texture);
    // load and generate the texture
    int width, height, nrChannels;
    //stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (data)
    {
        GLenum format;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        // set the texture wrapping/filtering options (on the currently bound texture object)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    return texture;
}