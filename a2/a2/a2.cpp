#define TINYOBJLOADER_IMPLEMENTATION
#include <iostream>
#include <vector>
#include <string>
#include "a2.h"
#include "tiny_obj_loader.h"

// Global variables to track control state
enum RotationAxis { ROT_X, ROT_Y, ROT_Z };
RotationAxis currentRotationAxis = ROT_Z;

enum ScalingDirection { SCALE_X, SCALE_Y, SCALE_Z, SCALE_UNIFORM };
ScalingDirection currentScalingDir = SCALE_Z;

float lastKeyPressTime = 0.0f;
bool canRotateClockwise = true, canRotateCounterclockwise = true;
bool wireframeMode = true; // Wireframe mode by default

std::vector<Vertex> loadModel(const std::string& path) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str())) {
        std::cerr << "Failed to load: " << err << std::endl;
        return std::vector<Vertex>();
    }

    std::vector<Vertex> vertices;

    // Loop over shapes
    for (const auto& shape : shapes) {
        // Loop over faces
        size_t index_offset = 0;
        for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++) {
            int fv = shape.mesh.num_face_vertices[f];

            // Loop over vertices in the face
            for (size_t v = 0; v < fv; v++) {
                tinyobj::index_t idx = shape.mesh.indices[index_offset + v];

                Vertex vertex;

                // Position
                vertex.position = {
                    attrib.vertices[3 * idx.vertex_index + 0],
                    attrib.vertices[3 * idx.vertex_index + 1],
                    attrib.vertices[3 * idx.vertex_index + 2]
                };

                // Normal
                if (idx.normal_index >= 0) {
                    vertex.normal = {
                        attrib.normals[3 * idx.normal_index + 0],
                        attrib.normals[3 * idx.normal_index + 1],
                        attrib.normals[3 * idx.normal_index + 2]
                    };
                }
                else {
                    vertex.normal = { 0.0f, 0.0f, 0.0f };
                }

                // Color (use a default color if not specified)
                if (idx.texcoord_index >= 0 && attrib.texcoords.size() > 0) {
                    // Use texture coordinate to generate color
                    vertex.color = {
                        attrib.texcoords[2 * idx.texcoord_index + 0],
                        attrib.texcoords[2 * idx.texcoord_index + 1],
                        0.5f  // Default blue component
                    };
                }
                else {
                    // Default color (white)
                    vertex.color = { 1.0f, 1.0f, 1.0f };
                }

                vertices.push_back(vertex);
            }
            index_offset += fv;
        }
    }

    std::cout << "Successfully loaded: " << path << std::endl;
    std::cout << "Loaded " << vertices.size() << " vertices" << std::endl;

    return vertices;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window, glm::mat4& model, float deltaTime, glm::vec3& rotationAxis, bool& wireframeMode) {
    float currentTime = glfwGetTime();
    bool canProcessKey = (currentTime - lastKeyPressTime) > KEY_REPEAT_DELAY;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    // Toggle wireframe mode with Tab
    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS && canProcessKey) {
        wireframeMode = !wireframeMode;
        lastKeyPressTime = currentTime;
        std::cout << "Switched to " << (wireframeMode ? "wireframe" : "solid") << " mode" << std::endl;
    }

    // Translation controls
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        model = glm::translate(model, glm::vec3(0.0f, TRANSLATION_DISTANCE, 0.0f));
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        model = glm::translate(model, glm::vec3(0.0f, -TRANSLATION_DISTANCE, 0.0f));
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        model = glm::translate(model, glm::vec3(-TRANSLATION_DISTANCE, 0.0f, 0.0f));
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        model = glm::translate(model, glm::vec3(TRANSLATION_DISTANCE, 0.0f, 0.0f));
    }

    // Rotation controls
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS && canRotateCounterclockwise) {
        glm::vec3 translation = glm::vec3(model[3]);
        model[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
        model = glm::rotate(model, glm::radians(ROTATION_ANGLE), rotationAxis);
        model[3] = glm::vec4(translation, 1.0f);
        canRotateCounterclockwise = false;
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && canRotateClockwise) {
        glm::vec3 translation = glm::vec3(model[3]);
        model[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
        model = glm::rotate(model, glm::radians(-ROTATION_ANGLE), rotationAxis);
        model[3] = glm::vec4(translation, 1.0f);
        canRotateClockwise = false;
    }
    if (glfwGetKey(window, GLFW_KEY_Q) != GLFW_PRESS) {
        canRotateCounterclockwise = true;
    }
    if (glfwGetKey(window, GLFW_KEY_E) != GLFW_PRESS) {
        canRotateClockwise = true;
    }

    // Scaling controls
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, SCALE_FACTOR));
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f / SCALE_FACTOR));
    }

    //// Reset model matrix
    //if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && canProcessKey) {
    //    model = glm::scale(glm::mat4(1.0f), glm::vec3(0.2f));
    //    std::cout << "Reset position, rotation, and scale." << std::endl;
    //    lastKeyPressTime = currentTime;
    //}
}

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Assignment 2 - 3D Model Viewer (Wireframe)", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    GLenum err = glewInit();
    if (GLEW_OK != err) {
        std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
        return -1;
    }

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Controls:" << std::endl;
    std::cout << "  W/S/A/D - Move up/down/left/right" << std::endl;
    std::cout << "  Q/E - Rotate around Y axis" << std::endl;
    std::cout << "  R/F - Scale in the Z axis" << std::endl;
    std::cout << "  Space - Reset to initial position" << std::endl;
    std::cout << "  Tab - Toggle wireframe mode" << std::endl;
    std::cout << "  Esc - Exit" << std::endl;

    // Compile shaders
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Load model
    std::vector<Vertex> vertices = loadModel("../cybertruck.obj");
    if (vertices.empty()) {
        std::cerr << "Failed to load model" << std::endl;
        return -1;
    }

    // Create vertex buffer and array objects
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    // Position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    // Color attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));

    // Normal attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST);

    // Set up camera and projection
    glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
    glm::mat4 view = glm::lookAt(
        glm::vec3(0.0f, 0.3f, 4.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );
    glm::mat4 projection = glm::perspective(
        glm::radians(45.0f),
        (float)WIDTH / (float)HEIGHT,
        0.1f,
        100.0f
    );

    glm::vec3 rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f);

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    model = glm::scale(model, glm::vec3(0.2f)); // Scale down first
    model = glm::translate(model, glm::vec3(0.0f, -12.0f, 0.0f)); // Then move down
    model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Initial rotation

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window, model, deltaTime, rotationAxis, wireframeMode);

        glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Set wireframe or solid mode
        if (wireframeMode) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glLineWidth(1.0f); // Set line width for wireframe
        }
        else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

        glUseProgram(shaderProgram);

        // Set lighting uniforms
        glm::vec3 lightPos = glm::vec3(1.5f, 1.5f, 1.5f);
        glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
        glm::vec3 viewPos = glm::vec3(0.0f, 0.3f, 2.0f);

        glUniform3fv(glGetUniformLocation(shaderProgram, "lightPos"), 1, glm::value_ptr(lightPos));
        glUniform3fv(glGetUniformLocation(shaderProgram, "viewPos"), 1, glm::value_ptr(viewPos));
        glUniform3fv(glGetUniformLocation(shaderProgram, "lightColor"), 1, glm::value_ptr(lightColor));

        // Set transform matrices
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        // Draw model
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    glfwTerminate();
    return 0;
}