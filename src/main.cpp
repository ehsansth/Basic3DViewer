#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader_s.h"

#include <iostream>
#include <cmath>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// window settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    // GLFW initialization
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    // Set GLFW window hints
    // OpenGL version 3.3 Core Profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Basic3DViewer", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // Initialize GLAD
    // Load all OpenGL function pointers using GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Build and compile the shader program
    Shader ourShader("shaders/3.3.shader.vs", "shaders/3.3.shader.fs");
    
    // Set up vertex data (and buffer(s)) and configure vertex attributes
    // Vertex data for a rectangle
    // The rectangle is defined by 4 vertices (2 triangles)
    float vertices[] = {
        // positions         // colors
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
    };   

    // Generate and bind a Vertex Buffer Object (VBO)
    // Generate a buffer ID
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind the Vertex Array Object first, then bind and set vertex buffer(s)
    glBindVertexArray(VAO);

    // Bind the buffer to the GL_ARRAY_BUFFER target
    // This tells OpenGL that we want to use the buffer as a vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Copy the vertex data into the buffer's memory
    // The first parameter is the target buffer, the second is the size of the data,
    // the third is a pointer to the data, and the last is the usage pattern
    // GL_STATIC_DRAW indicates that the data will not change
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    // 3. then set our vertex attributes pointers
     /*Tells Intrepret vertex data
     first parammeter is which vertex attribute to configure,
     size of the vertex attribute
     vertex attribute
     whether the vertex attribute should be normalized
     the stride: the space between consecutive vertex attributes
     offset of the first component of the vertex attribute in the buffer
     last parameter is the pointer to the vertex attribute data */ 
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);
    
    // Unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(VAO); 
    

    // Render loop
    while(!glfwWindowShouldClose(window))
    {
        // Call processInput function to handle user input
        processInput(window);

        // Rendering commands
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Activate the shader program
        ourShader.use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        
        // Swap the buffers & check and call for events
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }

    // de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}

// This function is called whenever the window is resized
// It adjusts the viewport to match the new window size
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// Process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}