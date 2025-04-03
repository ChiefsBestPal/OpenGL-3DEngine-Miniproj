#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/GL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

// Window dimensions
const unsigned int WIDTH = 1280;
const unsigned int HEIGHT = 720;

// Constants for transformations
const float TRANSLATION_DISTANCE = 0.01f;
const float ROTATION_ANGLE = 1.0f;
const float SCALE_FACTOR = 1.003f;
const float KEY_REPEAT_DELAY = 0.5f;

// Vertex shader with lighting
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"layout (location = 2) in vec3 aNormal;\n"
"out vec3 FragPos;\n"
"out vec3 Normal;\n"
"out vec3 ourColor;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main()\n"
"{\n"
"   FragPos = vec3(model * vec4(aPos, 1.0));\n"
"   Normal = mat3(transpose(inverse(model))) * aNormal;\n"
"   ourColor = aColor;\n"
"   gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
"}\0";

// Fragment shader with lighting
const char* fragmentShaderSource = "#version 330 core\n"
"in vec3 FragPos;\n"
"in vec3 Normal;\n"
"in vec3 ourColor;\n"
"out vec4 FragColor;\n"
"uniform vec3 lightPos;\n"
"uniform vec3 viewPos;\n"
"uniform vec3 lightColor;\n"
"void main()\n"
"{\n"
"   float ambientStrength = 0.3;\n"
"   vec3 ambient = ambientStrength * lightColor;\n"
"   \n"
"   vec3 norm = normalize(Normal);\n"
"   vec3 lightDir = normalize(lightPos - FragPos);\n"
"   float diff = max(dot(norm, lightDir), 0.0);\n"
"   vec3 diffuse = diff * lightColor;\n"
"   \n"
"   float specularStrength = 0.5;\n"
"   vec3 viewDir = normalize(viewPos - FragPos);\n"
"   vec3 reflectDir = reflect(-lightDir, norm);\n"
"   float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);\n"
"   vec3 specular = specularStrength * spec * lightColor;\n"
"   \n"
"   vec3 result = (ambient + diffuse + specular) * ourColor;\n"
"   FragColor = vec4(result, 1.0);\n"
"}\n\0";

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 color;
};

// Function prototypes
std::vector<Vertex> loadModel(const std::string& path);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, glm::mat4& model, float deltaTime, glm::vec3& rotationAxis, bool& wireframeMode);