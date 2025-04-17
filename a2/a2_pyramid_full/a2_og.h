#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/GL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Window dimensions
// Used for framebuffer size callback and aspect ratio calculations
const unsigned int WIDTH = 1280;
const unsigned int HEIGHT = 720;

// Constants for transformations - tuned for smooth draw updates based on geometric transformation at play
const float TRANSLATION_DISTANCE = 0.001f;
const float ROTATION_ANGLE = 1.0f;
const float SCALE_FACTOR = 1.003f;

// Time-safety-control control - provides time to register key inputs for proper visualization of transformations
const float KEY_REPEAT_DELAY = 0.5f;

// Vertex shader with normal-plane based lighting effects
// Uses MVP for model->world->view->clip space transformations
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

// Fragment shader with normal-plane based lighting calculations
// Used to determine in a scene potential final rasterized pixel colors
// In view, 
// ambient lighting is a constant light that affects all objects in the scene as perceived e.g. moonlight
// specular lighting is calculated based on the reflection of light rays off the surface e.g. shiny objects with singular shiny/reflective highlighted spot
// diffuse lighting is calculated based on the angle between the normal vector and the light vector e.g. matte objects, light is scattered in all directions towards the viewer, uniform shadow
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
"   // Ambient lighting\n"
"   float ambientStrength = 0.3;\n"
"   vec3 ambient = ambientStrength * lightColor;\n"
"   \n"
"   // Diffuse lighting\n"
"   vec3 norm = normalize(Normal);\n"
"   vec3 lightDir = normalize(lightPos - FragPos);\n"
"   float diff = max(dot(norm, lightDir), 0.0);\n"
"   vec3 diffuse = diff * lightColor;\n"
"   \n"
"   // Specular lighting\n"
"   float specularStrength = 0.5;\n"
"   vec3 viewDir = normalize(viewPos - FragPos);\n"
"   vec3 reflectDir = reflect(-lightDir, norm);\n"
"   float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);\n"
"   vec3 specular = specularStrength * spec * lightColor;\n"
"   \n"
"   // Combine results\n"
"   vec3 result = (ambient + diffuse + specular) * ourColor;\n"
"   FragColor = vec4(result, 1.0);\n"
"}\n\0";

// Function Prototypes declarations

// Callback for a window resize GLFW event
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// Process keyboard input for transformations
void processInput(GLFWwindow* window, glm::mat4& model, float deltaTime, glm::vec3& rotationAxis, bool& wireframeMode);

//#pragma once
//#include <GL/glew.h>
//#include <GLFW/glfw3.h>
//#include <GL/GL.h>
//
//const unsigned int WIDTH = 1280;
//const unsigned int HEIGHT = 720;
//
//const char* vertexShaderSource = "#version 330 core\n"
//"layout (location = 0) in vec3 aPos;\n"
//"void main()\n"
//"{"
//"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
//"}\0";
//const char* fragmentShaderSource = "#version 330 core\n"
//"out vec4 FragColor;\n"
//"void main()\n"
//"{\n"
//"   FragColor = vec4(0.6f, 0.84f, 0.87f, 1.0f);\n"
//"}\n\0";
//
//void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//void processInput(GLFWwindow* window);
