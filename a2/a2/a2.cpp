#include <iostream>
#include <vector>
#include <string>
#include "a2.h"

// Global variables to track control state
enum RotationAxis { ROT_X, ROT_Y, ROT_Z };
RotationAxis currentRotationAxis = ROT_Z;

enum ScalingDirection { SCALE_X, SCALE_Y, SCALE_Z, SCALE_UNIFORM };
ScalingDirection currentScalingDir = SCALE_UNIFORM;

// Timing for key input
float lastKeyPressTime = 0.0f;

// Callback function for window resize
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// Function to process keyboard input
void processInput(GLFWwindow* window, glm::mat4& model, float deltaTime, glm::vec3& rotationAxis, bool& wireframeMode) {
    float currentTime = glfwGetTime();

    // Check if enough time has passed since last key press
    bool canProcessKey = (currentTime - lastKeyPressTime) > KEY_REPEAT_DELAY;

    // Exit application
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    // Translation controls - continuous movement
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        model = glm::translate(model, glm::vec3(0.0f, TRANSLATION_DISTANCE, 0.0f));  // Move up
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        model = glm::translate(model, glm::vec3(0.0f, -TRANSLATION_DISTANCE, 0.0f)); // Move down
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        model = glm::translate(model, glm::vec3(-TRANSLATION_DISTANCE, 0.0f, 0.0f)); // Move left
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        model = glm::translate(model, glm::vec3(TRANSLATION_DISTANCE, 0.0f, 0.0f));  // Move right
    }

    // Rotation controls
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        // Rotate counterclockwise around current axis
        model = glm::rotate(model, glm::radians(ROTATION_ANGLE * deltaTime * 50.0f), rotationAxis);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        // Rotate clockwise around current axis
        model = glm::rotate(model, glm::radians(-ROTATION_ANGLE * deltaTime * 50.0f), rotationAxis);
    }

    // Scaling controls
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        // Scale up based on current scaling direction
        switch (currentScalingDir) {
        case SCALE_X:
            model = glm::scale(model, glm::vec3(SCALE_FACTOR, 1.0f, 1.0f));
            break;
        case SCALE_Y:
            model = glm::scale(model, glm::vec3(1.0f, SCALE_FACTOR, 1.0f));
            break;
        case SCALE_Z:
            model = glm::scale(model, glm::vec3(1.0f, 1.0f, SCALE_FACTOR));
            break;
        case SCALE_UNIFORM:
            model = glm::scale(model, glm::vec3(SCALE_FACTOR, SCALE_FACTOR, SCALE_FACTOR));
            break;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
        // Scale down based on current scaling direction
        switch (currentScalingDir) {
        case SCALE_X:
            model = glm::scale(model, glm::vec3(1.0f / SCALE_FACTOR, 1.0f, 1.0f));
            break;
        case SCALE_Y:
            model = glm::scale(model, glm::vec3(1.0f, 1.0f / SCALE_FACTOR, 1.0f));
            break;
        case SCALE_Z:
            model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f / SCALE_FACTOR));
            break;
        case SCALE_UNIFORM:
            model = glm::scale(model, glm::vec3(1.0f / SCALE_FACTOR, 1.0f / SCALE_FACTOR, 1.0f / SCALE_FACTOR));
            break;
        }
    }

    // Toggle wireframe mode
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS && canProcessKey) {
        wireframeMode = !wireframeMode;
        lastKeyPressTime = currentTime;
    }

    // Switch rotation axis
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS && canProcessKey) {
        // Cycle through rotation axes: X -> Y -> Z -> X
        switch (currentRotationAxis) {
        case ROT_X:
            currentRotationAxis = ROT_Y;
            rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f); // Y-axis
            std::cout << "Rotation axis: Y" << std::endl;
            break;
        case ROT_Y:
            currentRotationAxis = ROT_Z;
            rotationAxis = glm::vec3(0.0f, 0.0f, 1.0f); // Z-axis
            std::cout << "Rotation axis: Z" << std::endl;
            break;
        case ROT_Z:
            currentRotationAxis = ROT_X;
            rotationAxis = glm::vec3(1.0f, 0.0f, 0.0f); // X-axis
            std::cout << "Rotation axis: X" << std::endl;
            break;
        }
        lastKeyPressTime = currentTime;
    }

    // Switch scaling direction
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS && canProcessKey) {
        // Cycle through scaling directions: X -> Y -> Z -> Uniform -> X
        switch (currentScalingDir) {
        case SCALE_X:
            currentScalingDir = SCALE_Y;
            std::cout << "Scaling direction: Y" << std::endl;
            break;
        case SCALE_Y:
            currentScalingDir = SCALE_Z;
            std::cout << "Scaling direction: Z" << std::endl;
            break;
        case SCALE_Z:
            currentScalingDir = SCALE_UNIFORM;
            std::cout << "Scaling direction: Uniform" << std::endl;
            break;
        case SCALE_UNIFORM:
            currentScalingDir = SCALE_X;
            std::cout << "Scaling direction: X" << std::endl;
            break;
        }
        lastKeyPressTime = currentTime;
    }

    // Reset model matrix to identity
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && canProcessKey) {
        model = glm::mat4(1.0f);
        std::cout << "Reset position, rotation, and scale." << std::endl;
        lastKeyPressTime = currentTime;
    }
}

int main() {
    // Step 1: Setting Up Your Environment
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
	//glutInit();
    
    // Configure GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Assignment 2 - Enhanced 3D Pyramid", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Initialize GLEW
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
        return -1;
    }

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Controls:" << std::endl;
    std::cout << "  W/S/A/D - Move up/down/left/right" << std::endl;
    std::cout << "  Q/E - Rotate around current axis" << std::endl;
    std::cout << "  R/F - Scale in current direction" << std::endl;
    std::cout << "  X - Switch rotation axis (X, Y, Z)" << std::endl;
    std::cout << "  C - Switch scaling direction (X, Y, Z, Uniform)" << std::endl;
    std::cout << "  T - Toggle wireframe mode" << std::endl;
    std::cout << "  Space - Reset to initial position" << std::endl;
    std::cout << "  Esc - Exit" << std::endl;

    // Step 2: Drawing Basic Shapes
    // Compile vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Check for vertex shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Compile fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Check for fragment shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // Delete shaders as they're linked into our program now and no longer needed
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Define the vertices of a pyramid with positions, colors, and accurate normals for better lighting
    float pyramid_vertices[] = {
        // Positions            // Colors               // Normals
        // Base vertices
        -0.5f, -0.5f, -0.5f,    0.9f, 0.2f, 0.2f,      0.0f, -1.0f, 0.0f,  // base vertex 0 - red
         0.5f, -0.5f, -0.5f,    0.2f, 0.9f, 0.2f,      0.0f, -1.0f, 0.0f,  // base vertex 1 - green
         0.5f, -0.5f,  0.5f,    0.2f, 0.2f, 0.9f,      0.0f, -1.0f, 0.0f,  // base vertex 2 - blue
        -0.5f, -0.5f,  0.5f,    0.9f, 0.9f, 0.2f,      0.0f, -1.0f, 0.0f,  // base vertex 3 - yellow

        // Apex with different positions for different normals (for proper lighting)
        0.0f,  0.5f,  0.0f,    0.9f, 0.2f, 0.9f,      0.0f, 0.5f, -0.866f,  // apex for front face
        0.0f,  0.5f,  0.0f,    0.9f, 0.2f, 0.9f,      0.866f, 0.5f, 0.0f,   // apex for right face
        0.0f,  0.5f,  0.0f,    0.9f, 0.2f, 0.9f,      0.0f, 0.5f, 0.866f,   // apex for back face
        0.0f,  0.5f,  0.0f,    0.9f, 0.2f, 0.9f,      -0.866f, 0.5f, 0.0f,  // apex for left face
    };

    // Indices for the pyramid (triangles)
    unsigned int indices[] = {
        // Base (made of 2 triangles)
        0, 1, 2,    // first triangle of base
        0, 2, 3,    // second triangle of base
        // Sides (4 triangles connecting base to apex)
        0, 1, 4,    // front face
        1, 2, 5,    // right face
        2, 3, 6,    // back face
        3, 0, 7     // left face
    };

    // Generate and bind VAO, VBO, and EBO
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Bind VAO first
    glBindVertexArray(VAO);

    // Bind and set VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pyramid_vertices), pyramid_vertices, GL_STATIC_DRAW);

    // Bind and set EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Position attribute (3 float values per vertex)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Color attribute (3 float values per vertex)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Normal attribute (3 float values per vertex)
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Unbind VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Enable depth testing to properly render 3D objects
    glEnable(GL_DEPTH_TEST);

    // Step 3: Set up camera and projection
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::lookAt(
        glm::vec3(0.0f, 0.3f, 2.0f),    // Camera position
        glm::vec3(0.0f, 0.0f, 0.0f),    // Look at center
        glm::vec3(0.0f, 1.0f, 0.0f)     // Up vector
    );
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

    // Current rotation axis (start with Z-axis)
    glm::vec3 rotationAxis = glm::vec3(0.0f, 0.0f, 1.0f);

    // Wireframe mode toggle
    bool wireframeMode = false;

    // For smooth animation, add timing between frames
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    // Initial rotation to make the pyramid clearly visible
    model = glm::rotate(model, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    // Main render loop
    while (!glfwWindowShouldClose(window)) {
        // Calculate delta time for smooth movement
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Process input to update transforms
        processInput(window, model, deltaTime, rotationAxis, wireframeMode);

        // Clear screen and depth buffer
        glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Set polygon mode based on wireframe toggle
        if (wireframeMode)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        // Use our shader program
        glUseProgram(shaderProgram);

        // Set lighting uniforms
        glm::vec3 lightPos = glm::vec3(1.5f, 1.5f, 1.5f);
        glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
        glm::vec3 viewPos = glm::vec3(0.0f, 0.3f, 2.0f);  // Same as camera position

        // Get uniform locations
        unsigned int lightPosLoc = glGetUniformLocation(shaderProgram, "lightPos");
        unsigned int viewPosLoc = glGetUniformLocation(shaderProgram, "viewPos");
        unsigned int lightColorLoc = glGetUniformLocation(shaderProgram, "lightColor");

        // Set uniform values
        glUniform3fv(lightPosLoc, 1, glm::value_ptr(lightPos));
        glUniform3fv(viewPosLoc, 1, glm::value_ptr(viewPos));
        glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));

        // Set transform matrices
        unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
        unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
        unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // Bind VAO and draw
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);

        //// Auto-rotate for better visualization when idle
        //if (!glfwGetKey(window, GLFW_KEY_Q) && !glfwGetKey(window, GLFW_KEY_E)) {
        //    model = glm::rotate(model, glm::radians(0.1f), rotationAxis);
        //}

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}
//#include <iostream>
//#include <vector>
//#include "a2.h"
//
//int main() {
//    // Step 1: Setting Up Your Environment
//    // Initialize GLFW
//    if (!glfwInit()) {
//        std::cerr << "Failed to initialize GLFW" << std::endl;
//        return -1;
//    }
//
//    // Configure GLFW
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//    // Create window
//    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Assignment 2 - Pyramid", NULL, NULL);
//    if (window == NULL) {
//        std::cerr << "Failed to create GLFW window" << std::endl;
//        glfwTerminate();
//        return -1;
//    }
//
//    glfwMakeContextCurrent(window);
//    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
//
//    // Initialize GLEW
//    GLenum err = glewInit();
//    if (GLEW_OK != err) {
//        std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
//        return -1;
//    }
//
//    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
//    std::cout << "Controls:" << std::endl;
//    std::cout << "  W/S/A/D - Move up/down/left/right" << std::endl;
//    std::cout << "  Q/E - Rotate counterclockwise/clockwise around Z axis" << std::endl;
//    std::cout << "  R/F - Scale in positive/negative Z direction" << std::endl;
//    std::cout << "  Space - Reset to initial position" << std::endl;
//    std::cout << "  Esc - Exit" << std::endl;
//
//    // Step 2: Drawing Basic Shapes
//    // Compile vertex shader
//    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
//    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
//    glCompileShader(vertexShader);
//
//    // Check for vertex shader compile errors
//    int success;
//    char infoLog[512];
//    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
//    if (!success) {
//        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
//        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
//    }
//
//    // Compile fragment shader
//    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
//    glCompileShader(fragmentShader);
//
//    // Check for fragment shader compile errors
//    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
//    if (!success) {
//        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
//        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
//    }
//
//    // Link shaders
//    unsigned int shaderProgram = glCreateProgram();
//    glAttachShader(shaderProgram, vertexShader);
//    glAttachShader(shaderProgram, fragmentShader);
//    glLinkProgram(shaderProgram);
//
//    // Check for linking errors
//    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
//    if (!success) {
//        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
//        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
//    }
//
//    // Delete shaders as they're linked into our program now and no longer needed
//    glDeleteShader(vertexShader);
//    glDeleteShader(fragmentShader);
//
//    // Define the vertices of a pyramid (5 vertices: 4 for base and 1 for apex)
//    // Each vertex has position (x,y,z) and color (r,g,b)
//    float pyramid_vertices[] = {
//        // Positions            // Colors
//        // Base vertices
//        -0.5f, -0.5f, -0.5f,    1.0f, 0.0f, 0.0f,  // base vertex 0 - red
//         0.5f, -0.5f, -0.5f,    0.0f, 1.0f, 0.0f,  // base vertex 1 - green
//         0.5f, -0.5f,  0.5f,    0.0f, 0.0f, 1.0f,  // base vertex 2 - blue
//        -0.5f, -0.5f,  0.5f,    1.0f, 1.0f, 0.0f,  // base vertex 3 - yellow
//         0.0f,  0.5f,  0.0f,    1.0f, 0.0f, 1.0f   // apex - magenta
//    };
//
//    // Indices for the pyramid (triangles)
//    unsigned int indices[] = {
//        // Base (made of 2 triangles)
//        0, 1, 2,    // first triangle of base
//        0, 2, 3,    // second triangle of base
//        // Sides (4 triangles connecting base to apex)
//        0, 1, 4,    // front face
//        1, 2, 4,    // right face
//        2, 3, 4,    // back face
//        3, 0, 4     // left face
//    };
//
//    // Generate and bind VAO, VBO, and EBO
//    unsigned int VAO, VBO, EBO;
//    glGenVertexArrays(1, &VAO);
//    glGenBuffers(1, &VBO);
//    glGenBuffers(1, &EBO);
//
//    // Bind VAO first
//    glBindVertexArray(VAO);
//
//    // Bind and set VBO
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(pyramid_vertices), pyramid_vertices, GL_STATIC_DRAW);
//
//    // Bind and set EBO
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
//
//    // Position attribute (3 float values per vertex)
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);
//
//    // Color attribute (3 float values per vertex)
//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
//    glEnableVertexAttribArray(1);
//
//    // Unbind VBO and VAO
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//    glBindVertexArray(0);
//
//    // Enable depth testing to properly render 3D objects
//    glEnable(GL_DEPTH_TEST);
//
//    // Step 3: Applying Transformations
//    // Create transformation matrix - initially the identity matrix
//    glm::mat4 transform = glm::mat4(1.0f);
//
//    // For smooth animation, add timing between frames
//    float deltaTime = 0.0f;
//    float lastFrame = 0.0f;
//
//    // Main render loop
//    while (!glfwWindowShouldClose(window)) {
//        // Calculate delta time for smooth movement
//        float currentFrame = glfwGetTime();
//        deltaTime = currentFrame - lastFrame;
//        lastFrame = currentFrame;
//
//        // Process input to update transforms
//        processInput(window, transform, deltaTime);
//
//        // Clear screen and depth buffer
//        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//        // Use our shader program
//        glUseProgram(shaderProgram);
//
//        // Pass transformation matrix to shader
//        unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
//        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
//
//        // Draw pyramid
//        glBindVertexArray(VAO);
//        glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);
//
//        // Swap buffers and poll events
//        glfwSwapBuffers(window);
//        glfwPollEvents();
//    }
//
//    // Clean up resources
//    glDeleteVertexArrays(1, &VAO);
//    glDeleteBuffers(1, &VBO);
//    glDeleteBuffers(1, &EBO);
//    glDeleteProgram(shaderProgram);
//
//    // Terminate GLFW
//    glfwTerminate();
//    return 0;
//}
//
//void processInput(GLFWwindow* window, glm::mat4& transform, float deltaTime) {
//    // Exit program if ESC key is pressed
//    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//        glfwSetWindowShouldClose(window, true);
//
//    // Track time since last transformation
//    static float timeSinceLastTransform = 0.0f;
//    timeSinceLastTransform += deltaTime;
//
//    // Only apply transformations after a certain delay
//    bool canTransform = timeSinceLastTransform > KEY_REPEAT_DELAY;
//
//    // Reset transformation when Space key is pressed
//    static bool spaceReleased = true;
//    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && spaceReleased) {
//        transform = glm::mat4(1.0f); // Reset to identity matrix
//        spaceReleased = false;
//        std::cout << "Pyramid reset to initial position" << std::endl;
//    }
//    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) {
//        spaceReleased = true;
//    }
//
//    // If we can't transform yet, return early
//    if (!canTransform) {
//        return;
//    }
//
//    // Step 4: Implement Keyboard Input for Transformations
//    bool transformApplied = false;
//
//    // Translation controls
//    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
//        // Translate up
//        transform = glm::translate(transform, glm::vec3(0.0f, TRANSLATION_DISTANCE, 0.0f));
//        transformApplied = true;
//    }
//    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
//        // Translate down
//        transform = glm::translate(transform, glm::vec3(0.0f, -TRANSLATION_DISTANCE, 0.0f));
//        transformApplied = true;
//    }
//    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
//        // Translate left
//        transform = glm::translate(transform, glm::vec3(-TRANSLATION_DISTANCE, 0.0f, 0.0f));
//        transformApplied = true;
//    }
//    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
//        // Translate right
//        transform = glm::translate(transform, glm::vec3(TRANSLATION_DISTANCE, 0.0f, 0.0f));
//        transformApplied = true;
//    }
//
//    // Rotation controls
//    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
//        // Rotate around Z-axis counterclockwise
//        transform = glm::rotate(transform, glm::radians(ROTATION_ANGLE), glm::vec3(0.0f, 0.0f, 1.0f));
//        transformApplied = true;
//    }
//    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
//        // Rotate around Z-axis clockwise
//        transform = glm::rotate(transform, glm::radians(-ROTATION_ANGLE), glm::vec3(0.0f, 0.0f, 1.0f));
//        transformApplied = true;
//    }
//
//    // Scaling controls
//    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
//        // Scale in positive Z direction
//        transform = glm::scale(transform, glm::vec3(1.0f, 1.0f, SCALE_FACTOR));
//        transformApplied = true;
//    }
//    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
//        // Scale in negative Z direction
//        transform = glm::scale(transform, glm::vec3(1.0f, 1.0f, 1.0f / SCALE_FACTOR));
//        transformApplied = true;
//    }
//
//    // Reset timer if a transformation was applied
//    if (transformApplied) {
//        timeSinceLastTransform = 0.0f;
//    }
//}
//
//void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
//    // Adjust viewport when window size changes
//    glViewport(0, 0, width, height);
//}
//#include <iostream>
//#include "a2.h"
//
//int main() {
//	glfwInit();
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Assignment 2", NULL, NULL);
//	glfwMakeContextCurrent(window);
//	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
//
//	GLenum err = glewInit();
//	if (GLEW_OK != err) {
//		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
//	}
//
//    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
//    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
//    glCompileShader(vertexShader);
//
//    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
//    glCompileShader(fragmentShader);
//
//    unsigned int shaderProgram = glCreateProgram();
//    glAttachShader(shaderProgram, vertexShader);
//    glAttachShader(shaderProgram, fragmentShader);
//    glLinkProgram(shaderProgram);
//
//    glDeleteShader(vertexShader);
//    glDeleteShader(fragmentShader);
//
//    float triangle_verices[] = {
//        -0.5f, -0.5f, 0.0f, // left  
//         0.5f, -0.5f, 0.0f, // right 
//         0.0f,  0.5f, 0.0f  // top   
//    };
//
//    unsigned int VBO, VAO;
//    glGenVertexArrays(1, &VAO);
//    glGenBuffers(1, &VBO);
//    glBindVertexArray(VAO);
//
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_verices), triangle_verices, GL_STATIC_DRAW);
//
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);
//
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//
//    glBindVertexArray(0);
//
//    // render loop
//    // -----------
//    while (!glfwWindowShouldClose(window))
//    {
//        // input
//        // -----
//        processInput(window);
//
//        // render
//        // ------
//        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT);
//
//        glUseProgram(shaderProgram);
//        glBindVertexArray(VAO);
//        glDrawArrays(GL_TRIANGLES, 0, 3);
//
//        glfwSwapBuffers(window);
//        glfwPollEvents();
//    }
//
//    glDeleteVertexArrays(1, &VAO);
//    glDeleteBuffers(1, &VBO);
//    glDeleteProgram(shaderProgram);
//
//    glfwTerminate();
//    return 0;
//}
//
//void processInput(GLFWwindow* window) {
//	//Keyboard input stuff here for step 4
//	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//		glfwSetWindowShouldClose(window, true);
//}
//
//void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
//	glViewport(0, 0, width, height);
//}