#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/GL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Window dimensions
const unsigned int WIDTH = 1280;
const unsigned int HEIGHT = 720;

// Constants for transformations - further reduced speeds for slower animation
const float TRANSLATION_DISTANCE = 0.005f;  // Reduced for slower movement
const float ROTATION_ANGLE = 1.0f;          // Reduced for slower rotation 
const float SCALE_FACTOR = 1.01f;           // Reduced for slower scaling

// Time-based control - only apply transformation after this many seconds
const float KEY_REPEAT_DELAY = 0.05f;       // Add delay between transformation applications

// Vertex shader with transformation support
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 ourColor;\n"
"uniform mat4 transform;\n"
"void main()\n"
"{\n"
"   gl_Position = transform * vec4(aPos, 1.0);\n"
"   ourColor = aColor;\n"
"}\0";

// Fragment shader that receives color from vertex shader
const char* fragmentShaderSource = "#version 330 core\n"
"in vec3 ourColor;\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(ourColor, 1.0f);\n"
"}\n\0";

// Function declarations
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, glm::mat4& transform, float deltaTime);
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