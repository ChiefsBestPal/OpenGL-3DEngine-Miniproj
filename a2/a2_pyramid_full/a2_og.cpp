#include <iostream>
#include <vector>
#include <string>
#include "a2_og.h"

// Global variables to track control state
enum RotationAxis { ROT_X, ROT_Y, ROT_Z };
RotationAxis currentRotationAxis = ROT_Z;

enum ScalingDirection { SCALE_X, SCALE_Y, SCALE_Z, SCALE_UNIFORM };
ScalingDirection currentScalingDir = SCALE_Z;

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
    // 
    // Compile vertex shader
	// Vertex shader processes each vertex of a primitive (point, line, triangle)
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
	// fragments are pixel candidates for the final image.
	// fragment shader runs for each fragment generated by the rasterizer.
	// GPU Rasterization turns 3D coordinates of a vertex into a 2D image space pixel.
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
	// Linking of the compiled shaders into a complete pipeline for a program object.
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

    // Delete individual shaders compiled assemblies since they are linked in shader program of context execution
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Define the vertices of a pyramid with positions, colors, and accurate normals for better lighting
	// the 4 Base vertices as well as multi-positioned apex vertex for different normals / base sides
    // Position: x, y, z coordinates of vertex
	// Color: r, g, b % values
	// Normal: x, y, z of direction vector perpendicular to the supposed surface plane of vertex
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

    // Indices for the pyramid (each triple is a triangle element )
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
	glGenVertexArrays(1, &VAO); // Vertex Array Object: stores vertex attribute configurations and which VBO to use
	glGenBuffers(1, &VBO); // Vertex Buffer Object: stores vertex data efficiently in GPU memory
	glGenBuffers(1, &EBO); // Element Buffer Object: stores indices (or elements/vertex group) that OpenGL uses to decide which vertices to draw

    // Bind VAO first
    glBindVertexArray(VAO);

	// Bind and set VBO; allocate static gpu memory for vertices
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pyramid_vertices), pyramid_vertices, GL_STATIC_DRAW);

	// Bind and set EBO; allocate static gpu memory for indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Vertex Attribute#0: Position (VAO floats 0,1,2);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Vertex Attribute#1: Color (VAO floats 3,4,5);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Vertex Attribute#2: Normals (VAO floats 6,7,8);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Unbind VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Unbind then VAO 
    glBindVertexArray(0);

    // Enable depth testing to properly render 3D objects within context
	// Z-buffering: a technique used to determine which objects are visible in a scene (Image space) based on their depth from the camera
	// Z-buffer GL_DEPTH_TEST: compares the depth of each fragment with the depth value stored in the depth buffer...
    // e.g. further objects should be more obscure when overlayed
    glEnable(GL_DEPTH_TEST);

    // Step 3: Set up camera and projection
	// Uses MVP (Model, View, Projection) matrices to transform from model space to clip space

	// Model R4: transform objects from model space to world space
    glm::mat4 model = glm::mat4(1.0f);
	// View R4: transform world space to camera space
    glm::mat4 view = glm::lookAt(
        glm::vec3(0.0f, 0.3f, 2.0f),    // Camera position
        glm::vec3(0.0f, 0.0f, 0.0f),    // Look at center
        glm::vec3(0.0f, 1.0f, 0.0f)     // Up vector
    );
	// Projection R4: transform camera space to clip space
    glm::mat4 projection = glm::perspective(
		glm::radians(45.0f),          // Field of view
		(float)WIDTH / (float)HEIGHT, // Aspect ratio
		0.1f,                         // Near clipping plane
		100.0f                        // Far clipping plane
    );

    // Current rotation axis (start with Z-axis)
    glm::vec3 rotationAxis = glm::vec3(0.0f, 0.0f, 1.0f);

    // Wireframe mode toggle
    bool wireframeMode = false;

    // For smooth animation, add timing between frames
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    // Initial rotation to make the pyramid clearly visible
    // TODO: WHAT INITIAL MODEL VIEW SHOULD BE USED?
    model = glm::rotate(model, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    // Main render loop
    while (!glfwWindowShouldClose(window)) {
        // Calculate delta time for smooth movement  and
        // delays to allow time to register inputs for proper visualizations of transforms
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Process input to update transforms
        processInput(window, model, deltaTime, rotationAxis, wireframeMode);

        // Clear screen set to bg color, clear color+ depth buffer using bitflags
        glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Set polygon mode based on wireframe toggle
		// Wireframe mode: Only draw the edges of the triangle elements
        if (wireframeMode)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		// Use linked shader program (vertex and fragment shaders) as part of current rendering pipeline
        glUseProgram(shaderProgram);

		// Uniforms for lighting effects
		// Uniforms: Variables from CPU passed to shaders GPU that remain constant for the entire draw call 
        // Set lighting uniforms
		glm::vec3 lightPos = glm::vec3(1.5f, 1.5f, 1.5f); // Light position in world space
		glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f); // White light
        glm::vec3 viewPos = glm::vec3(0.0f, 0.3f, 2.0f);  // Try Same as camera position

        // Get uniform lighting locations
        unsigned int lightPosLoc = glGetUniformLocation(shaderProgram, "lightPos");
        unsigned int viewPosLoc = glGetUniformLocation(shaderProgram, "viewPos");
        unsigned int lightColorLoc = glGetUniformLocation(shaderProgram, "lightColor");

        // Set uniform lighting values
		// glUniform3fv: set uniform vec3 value ([lightPos, viewPos, lightColor])
        glUniform3fv(lightPosLoc, 1, glm::value_ptr(lightPos));
        glUniform3fv(viewPosLoc, 1, glm::value_ptr(viewPos));
        glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));

		// Get transform matrices unfiform locations
        unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
        unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
        unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
		// Set Uniform transform matrices values
		// glUniformMatrix4fv: set uniform mat4 value (transform matrices)
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // Bind VAO; then finally Draw
        glBindVertexArray(VAO);
        glDrawElements(
			GL_TRIANGLES, // Drawing mode: Triangles
			18, // Number of indices 6 triangles * 3 vertices = 18 (6 rows of 3-index points)
			GL_UNSIGNED_INT, // Type of indices
			0 // Offset of EBO i.e. Offset of indices defining triangle elements
        );

		// Swap buffers front and back buffers
		// Double buffering: 1 buffer for drawing, 1 buffer for displaying
        glfwSwapBuffers(window); //TODO: glfwSwapInterval(1)?
		// Process GLFW window events e.g. keyboard/mouse inputs
        glfwPollEvents();
    }

	// Free pipeline resources
	// clean up buffers and execution shaders binaries
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);
    //Terminate GLFW library processes
    glfwTerminate();
    return 0;
}
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
