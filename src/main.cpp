#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include "classes/Shader.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 900;
const unsigned int SCR_HEIGHT = 600;
constexpr float PI = 3.14159265359f;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Lab2", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    Shader sphereShader("src/shaders/VertexShader.txt", "src/shaders/fragmentSphereShader.txt");
    Shader torusShader("src/shaders/VertexShader.txt", "src/shaders/fragmentTorusShader.txt");
    Shader planeShader("src/shaders/VertexShader.txt", "src/shaders/fragmentPlaneShader.txt");

    glm::vec3 positions[] = {
    glm::vec3(-2.6f, 0.0f, 0.0f),  
    glm::vec3(-0.15f, 0.0f, 0.0f),   
    glm::vec3(2.4f, 0.0f, 0.0f)   
    };

    const int sphereLatitudeSegments = 30;
    const int sphereLongitudeSegments = 30;
    std::vector<float> sphereVertices;

    for (int i = 0; i <= sphereLatitudeSegments; ++i) { 
        float theta1 = i * PI / sphereLatitudeSegments;
        float theta2 = (i + 1) * PI / sphereLatitudeSegments;

        for (int j = 0; j <= sphereLongitudeSegments; ++j) {  
            float phi1 = j * 2 * PI / sphereLongitudeSegments;
            float phi2 = (j + 1) * 2 * PI / sphereLongitudeSegments;

            float x1 = 0.5f * sin(theta1) * cos(phi1);
            float y1 = 0.5f * cos(theta1);
            float z1 = 0.5f * sin(theta1) * sin(phi1);
            sphereVertices.push_back(x1);
            sphereVertices.push_back(y1);
            sphereVertices.push_back(z1);

            float x2 = 0.5f * sin(theta2) * cos(phi1);
            float y2 = 0.5f * cos(theta2);
            float z2 = 0.5f * sin(theta2) * sin(phi1);
            sphereVertices.push_back(x2);
            sphereVertices.push_back(y2);
            sphereVertices.push_back(z2);

            float x3 = 0.5f * sin(theta2) * cos(phi2);
            float y3 = 0.5f * cos(theta2);
            float z3 = 0.5f * sin(theta2) * sin(phi2);
            sphereVertices.push_back(x3);
            sphereVertices.push_back(y3);
            sphereVertices.push_back(z3);

            sphereVertices.push_back(x1);
            sphereVertices.push_back(y1);
            sphereVertices.push_back(z1);

            sphereVertices.push_back(x3);
            sphereVertices.push_back(y3);
            sphereVertices.push_back(z3);

            float x4 = 0.5f * sin(theta1) * cos(phi2);
            float y4 = 0.5f * cos(theta1);
            float z4 = 0.5f * sin(theta1) * sin(phi2);
            sphereVertices.push_back(x4);
            sphereVertices.push_back(y4);
            sphereVertices.push_back(z4);
        }
    }

    const int torusMajorSegments = 40;  
    const int torusMinorSegments = 40;  
    const float torusMajorRadius = 0.7f;
    const float torusMinorRadius = 0.3f;

    std::vector<float> torusVertices;

    for (int i = 0; i < torusMajorSegments; ++i) {
        float theta = i * 2 * PI / torusMajorSegments;
        float nextTheta = (i + 1) * 2 * PI / torusMajorSegments;
        float cosTheta = cos(theta);
        float sinTheta = sin(theta);
        float cosNextTheta = cos(nextTheta);
        float sinNextTheta = sin(nextTheta);

        for (int j = 0; j <= torusMinorSegments; ++j) {
            float phi = j * 2 * PI / torusMinorSegments;
            float cosPhi = cos(phi);
            float sinPhi = sin(phi);

            float x = (torusMajorRadius + torusMinorRadius * cosPhi) * cosTheta;
            float y = (torusMajorRadius + torusMinorRadius * cosPhi) * sinTheta;
            float z = torusMinorRadius * sinPhi;
            torusVertices.push_back(x);
            torusVertices.push_back(y);
            torusVertices.push_back(z);

            float nextX = (torusMajorRadius + torusMinorRadius * cosPhi) * cosNextTheta;
            float nextY = (torusMajorRadius + torusMinorRadius * cosPhi) * sinNextTheta;
            float nextZ = torusMinorRadius * sinPhi;
            torusVertices.push_back(nextX);
            torusVertices.push_back(nextY);
            torusVertices.push_back(nextZ);
        }
    }

    float plane[6500];
    int k = 0;
    int num_of_steps = 20;
    for (int i = 0; i < num_of_steps - 1; i++) {
        for (int j = 0; j < num_of_steps - 1; j++) {
            float x1 = -1 + i * (2.0f / num_of_steps);
            float y1 = -1 + j * (2.0f / num_of_steps);
            float x2 = -1 + (i + 1) * (2.0f / num_of_steps);
            float y2 = -1 + (j + 1) * (2.0f / num_of_steps);

            plane[k++] = x1;
            plane[k++] = y1;
            plane[k++] = sqrt(fabs(x1 * y1));  

            plane[k++] = x2;
            plane[k++] = y1;
            plane[k++] = sqrt(fabs(x2 * y1)); 

            plane[k++] = x1;
            plane[k++] = y2;
            plane[k++] = sqrt(fabs(x1 * y2));

            plane[k++] = x2;
            plane[k++] = y1;
            plane[k++] = sqrt(fabs(x2 * y1)); 

            plane[k++] = x1;
            plane[k++] = y2;
            plane[k++] = sqrt(fabs(x1 * y2)); 

            plane[k++] = x2;
            plane[k++] = y2;
            plane[k++] = sqrt(fabs(x2 * y2));
        }
    }

    unsigned int VBOs[3], VAOs[3];
    glGenVertexArrays(3, VAOs);
    glGenBuffers(3, VBOs);

    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(plane), plane, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sphereVertices.size() * sizeof(float), &sphereVertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(VAOs[2]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[2]);
    glBufferData(GL_ARRAY_BUFFER, torusVertices.size() * sizeof(float), &torusVertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    float rotate_side = 0.0f, rotate_up = 0.0f, vec_up = 1.0f;

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(1.0f, 0.85f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            rotate_up += 0.05f;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            rotate_up -= 0.05f;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            rotate_side -= 0.05f;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            rotate_side += 0.05f;
        }

        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
            projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        }
        if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
            projection = glm::ortho(-3.0f, 3.0f, -3.0f, 3.0f, 0.1f, 100.0f);
        }

        glm::mat4 view = glm::mat4(1.0f);
        float radius = 6.0f;
        float camX = static_cast<float>(sin(glm::radians(rotate_side)) * cos(glm::radians(rotate_up)) * radius);
        float camZ = static_cast<float>(cos(glm::radians(rotate_side)) * cos(glm::radians(rotate_up)) * radius);
        float camY = static_cast<float>(sin(glm::radians(rotate_up)) * radius);
        if ((sin(glm::radians(rotate_up)) == 1) || (sin(glm::radians(rotate_up)) == -1))
            vec_up *= -1;
        view = glm::lookAt(glm::vec3(camX, camY, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, vec_up, 0.0f));

        planeShader.use();
        planeShader.setMat4("view", view);
        planeShader.setMat4("projection", projection);
        glm::mat4 model = glm::translate(glm::mat4(1.0f), positions[1]);
        planeShader.setMat4("model", model);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_TRIANGLES, 0, (num_of_steps - 1) * (num_of_steps - 1) * 6);


        sphereShader.use();
        sphereShader.setMat4("view", view);
        sphereShader.setMat4("projection", projection);
        model = glm::translate(glm::mat4(1.0f), positions[0]);
        model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
        sphereShader.setMat4("model", model);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glBindVertexArray(VAOs[1]);
        glDrawArrays(GL_TRIANGLES, 0, sphereVertices.size() / 3);

        torusShader.use();
        torusShader.setMat4("view", view);
        torusShader.setMat4("projection", projection);
        model = glm::translate(glm::mat4(1.0f), positions[2]);
        torusShader.setMat4("model", model);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glBindVertexArray(VAOs[2]);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, torusVertices.size() / 3);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(3, VAOs);
    glDeleteBuffers(3, VBOs);
    sphereShader.deleteProgram();
    torusShader.deleteProgram();
    planeShader.deleteProgram();

    glfwTerminate();
    return 0;
}


void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}