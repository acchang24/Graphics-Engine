#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <fstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Test.h"

// Define a window's dimensions
#define WIDTH 800
#define HEIGHT 600

bool isWireFrame = false;
bool prevInput = false;

// Initializes the GLFW library, and any other things required for the engine
void Init()
{
    // Initialize the GLFW library
    glfwInit();
    // Support OpenGl Versions 3 to 4
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Use core-profile
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    // Add GLFW_OPENGL_FORWARD_COMPAT for Max OS for initialization to work
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
}

// Resizes and adjusts the viewport when the user changes the window size.
// Takes a pointer to a GLFWwindow and two ints for the new window dimensions.
// This is called whenever the window changes in size.
void FrameBufferSizeCallBack(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// Processes any keyboard, mouse, or controller inputs
// Takes a pointer to a GLFWwindow
void ProcessInput(GLFWwindow *window)
{
    // If the user presses the escape key, close the window
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        // Set glfwSetWindowShouldClose to true
        glfwSetWindowShouldClose(window, true);
    }

    // Toggles wireframe drawing
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && !prevInput)
    {
        prevInput = true;
        isWireFrame = !isWireFrame;
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_RELEASE && prevInput)
    {
        prevInput = false;
    }
}

int main()
{
    Init();

    // Create a window
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Graphics Engine", NULL, NULL);

    // If window creation unsuccessful, cleanup glfw and quit program
    if (!window)
    {
        std::cout << "Window creation failed" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Set the window's context to the current one
    glfwMakeContextCurrent(window);

    // Initialize GLAD before OpenGl function calls
    // Load the address of the OpenGL function pointers (OS-specific)
    // This gives a glfwGetProcAddress that defines the correct function based on the OS
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Create viewport
    // Sets the location of lower left corner (0, 0)
    // Sets width/height of rendering window to the size of GLFW window size
    glViewport(0, 0, WIDTH, HEIGHT);
    // Tell GLFW to call window resize function on every window resize
    // This registers the callback function
    glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallBack);

    // // Triangle vertex data
    // float vertices[] = {-0.5f, -0.5f, 0.0f,
    //                     0.5f, -0.5f, 0.0f,
    //                     0.0f, 0.5f, 0.0f};

    // Rectangle vertex data
    float vertices[] = {
        0.5f, 0.5f, 0.0f,   // top right
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f   // top left
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    // Save Vertex Shader code as a const char* for now
    const char *vertexShaderSource = "#version 420 core\n"
                                     "layout(location = 0) in vec3 position;\n"
                                     "out vec4 vertexColor;\n"
                                     "void main()\n"
                                     "{\n"
                                     "gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
                                     "vertexColor = vec4(0.2f, 0.6f, 1.0f, 1.0f);\n"
                                     "}\0";

    const char *fragmentShaderSource = "#version 420 core\n"
                                       "in vec4 vertexColor;\n"
                                       "out vec4 FragColor;\n"
                                       "void main()\n"
                                       "{\n"
                                       "FragColor = vertexColor;\n"
                                       "}\0";

    // Create a vertex shader object with glCreateShader and store its id as an int
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // Attach shader code to the shader object:
    // Takes the shader object to compile,
    // how many strings that are going to be passed in as source code,
    // the actual source code of the shader,
    // and an array of string lengths
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    // Compile the shader
    glCompileShader(vertexShader);

    // Check to see if shader compilation was successful:
    // Define an integer to indicate success
    int success = 0;
    // Storage container for any error messages
    char infoLog[512];
    // Get any errors
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "Vertex shader compilation failed" << infoLog << std::endl;
    }
    else
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "Vertex shader compilation success" << infoLog << std::endl;
    }

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Check to see if shader compilation was successful:
    // Define an integer to indicate success
    int success1 = 0;
    // Storage container for any error messages
    char infoLog1[512];
    // Get any errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success1);
    if (!success1)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog1);
        std::cout << "Fragment shader compilation failed" << infoLog1 << std::endl;
    }
    else
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog1);
        std::cout << "Fragment shader compilation success" << infoLog1 << std::endl;
    }

    // Shader program
    // Create a shader program and return an ID reference
    unsigned int shaderProgram = glCreateProgram();
    // Attatch the compiled shaders to the program object
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    // Link to shader program
    glLinkProgram(shaderProgram);
    // Check to see if program failed and retrieve the log
    int success2 = 0;
    char infoLog2[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success2);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog2);
        std::cout << "Shader program creation failed" << infoLog2 << std::endl;
    }
    else
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog2);
        std::cout << "Shader program creation success" << infoLog2 << std::endl;
    }

    // Activate program object with glUseProgram
    // Every shader/rednering call will use this program object and its shaders
    glUseProgram(shaderProgram);

    // Delete shader objects once they have been linked into the program object
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //////// INITIALIZATION CODE ////////
    // Vertex array object
    unsigned int vao;
    glGenVertexArrays(1, &vao);
    // Bind the the new vertex array object with glBindVertexArray first,
    // then set any vertex buffers, vertex attributes, and any index buffers
    glBindVertexArray(vao);

    // Generate a new buffer for vertex buffer
    unsigned int vbo;
    glGenBuffers(1, &vbo);
    // Bind the new buffer as a vertex buffer (GL_ARRAY_BUFFER)
    // and sets the newly created buffer to the GL_ARRAY_BUFFER target
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Copy user defined data into a buffer that is currently bound
    // Takes in the type of buffer that is bound,
    // the size of data to pass in to the buffer in bytes,
    // the actual data to send into the the buffer,
    // and set how the data is managed by the GPU:
    // GL_STREAM_DRAW: the data is set only once and used by the GPU at most a few times.
    // GL_STATIC_DRAW: the data is set only once and used many times.
    // GL_DYNAMIC_DRAW: the data is changed a lot and used many times.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Genrate a new buffer for index buffer (Element buffer object)
    unsigned int ebo;
    glGenBuffers(1, &ebo);

    // Copy index array in a element buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Set vertex attributes pointers
    //   Link Vertex Attributes with glVertexAttribPointer():
    // - First argument specifies which vertex attribute to configure.
    //   Specified the plocation of position vertex attribute with layout (location = 0) in the vertex shader
    //   This sets location of vertex attribute to 0, and so pass in 0 for the data of this vertex attribute
    // - Second argument specifies size of the vertex attribute. This attribute is vec3, so it takes 3 values
    // - Third argument specifies the type of the data, which in this case is a GL_Float (vec* in GLSL)
    // - Fourth argument specifies if the data is going to be normalized.
    // - Fifth argument is the stride, and defines the space between consecutive vertex attributes
    // - Last argument is type void*, and is the offset of where the position data begins in the buffer
    //   Since the position data is at the start of the data array, it can be 0
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

    // Enable vertex attribute, giving the vertex attribute location as its argument
    glEnableVertexAttribArray(0);

    // // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    // glBindBuffer(GL_ARRAY_BUFFER, 0);

    // // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    // glBindVertexArray(0);

    // Get a timestamp of the current time
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
    // A float to keep track of the time
    float timer = 0.0f;
    // int fps = 0;

    // Render loop
    // glfwWindowShouldClose() checks at the start of each
    // loop iteration to see if GLFW needs to be closed
    while (!glfwWindowShouldClose(window))
    {
        // Process inputs at start of frame
        ProcessInput(window);

        // Toggle wireframe draws
        if (isWireFrame)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

        // Render
        // Clear the screen at the start of each frame
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        // Clear only the color buffer for now
        glClear(GL_COLOR_BUFFER_BIT);

        // Get a time stamp of the current time and set that as the end time
        std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
        // Get the duration of the two time stamps
        double duration = static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count());
        // Cast to float and calculate the delta time
        float deltaTime = static_cast<float>(0.000000001 * duration);
        // Set the new starting time stamp to the current end time stamp
        start = end;

        // fps = static_cast<int>(1.0f / deltaTime);

        // Increment the timer by deltaTime
        timer += deltaTime;

        // Draw triangles
        // Set a shader program to use
        glUseProgram(shaderProgram);

        // Bind Vertex Array Object
        glBindVertexArray(vao);

        // // glDrawArrays to draw primitives using the active shader:
        // // - First argument takes the OpenGL primitive type to draw. In this case, draw triangles
        // // - Second argument specifies the starting index of the vertex array to draw
        // // - Last argument specifies how many vertices to draw, in this case it is 3
        // glDrawArrays(GL_TRIANGLES, 0, 3);

        //   glDrawElements takes indices from EBO currently bound to GL_ELEMENT_ARRAY_BUFFER target:
        // - First argument specifies the mode to draw in, in this case still triangles
        // - Second argument is the count or number of elements to draw
        // - Third argument is type of indices which is of GL_UNSIGNED_INT
        // - Last argument allows us to specify offset in EBO or pass in an index array
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // No need to unbind every time
        // glBindVertexArray(0);

        // std::cout << timer << std::endl;

        //  Swap buffer that contains render info and outputs it to the screen
        glfwSwapBuffers(window);
        // Check to see if any events are triggered (inputs) and updates the window state
        glfwPollEvents();
    }

    // De-allocate all resources
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteProgram(shaderProgram);

    // Clean and delete all of GLFW's resources that were allocated
    glfwTerminate();

    return 0;
}
