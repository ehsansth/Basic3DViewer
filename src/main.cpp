#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// window settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// vertex shader in the shader language GLSL (OpenGL Shading Language)
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

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
    // Vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // Attach the shader source code to the shader object
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    
    // Compile the shader
    glCompileShader(vertexShader);
    int success;
    char infoLog[512];
    // Check for compilation errors
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Fragment shader
    // Create a fragment shader object
    // Attach the shader source code to the shader object
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    // Compile the shader
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Link the shaders to create a shader program
    // Create a shader program object
    unsigned int shaderProgram = glCreateProgram();
    // Attach the vertex and fragment shaders to the program
    // This tells OpenGL to use the vertex shader and fragment shader
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    // Link the program
    // This combines the vertex and fragment shaders into a single program
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    // Delete the shader objects after linking
    //  frees up resources
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    // Set up vertex data (and buffer(s)) and configure vertex attributes
    // Vertex data for a rectangle
    // The rectangle is defined by 4 vertices (2 triangles)
    float vertices[] = 
    {
        0.5f,  0.5f, 0.0f,  // top right
        0.5f, -0.5f, 0.0f,  // bottom right
       -0.5f, -0.5f, 0.0f,  // bottom left
       -0.5f,  0.5f, 0.0f   // top left 
    };
    // The rectangle is defined by 2 triangles
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };

    // Generate and bind a Vertex Buffer Object (VBO)
    // Generate a buffer ID
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // Create element buffer object (EBO)
    unsigned int EBO;
    glGenBuffers(1, &EBO);

    // Bind the Vertex Array Object first, then bind and set vertex buffer(s)
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Copy the vertex data into the buffer's memory
    // The first parameter is the target buffer, the second is the size of the data,
    // the third is a pointer to the data, and the last is the usage pattern
    // GL_STATIC_DRAW indicates that the data will not change
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // Copy index array data into the buffer's memory
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    // 3. then set our vertex attributes pointers
     /*Tells Intrepret vertex data
     first parammeter is which vertex attribute to configure,
     size of the vertex attribute
     vertex attribute
     whether the vertex attribute should be normalized
     the stride: the space between consecutive vertex attributes
     offset of the first component of the vertex attribute in the buffer
     last parameter is the pointer to the vertex attribute data */ 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);  

    // Bind the buffer to the GL_ARRAY_BUFFER target
    // This tells OpenGL that we want to use the buffer as a vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    // Unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0); 
    

    // Render loop
    while(!glfwWindowShouldClose(window))
    {
        // Call processInput function to handle user input
        processInput(window);

        // Rendering commands
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Rectnagle drawing
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        
        // Swap the buffers & check and call for events
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }

    // de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

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