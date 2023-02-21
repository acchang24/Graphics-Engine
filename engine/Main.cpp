#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <fstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Test.h"

// Define a window's dimensions
#define WIDTH 1280
#define HEIGHT 720

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
}

int main()
{
    Init();

    std::ifstream inFile("Input/test.txt");

    if (inFile.is_open())
    {
        std::string line;
        while (std::getline(inFile, line))
        {
            std::cout << line << std::endl;
        }
    }
    else
    {
        std::cout << "Can't open file" << std::endl;
    }

    // // Triangle vertex data
    // float vertices[] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f};

    // // Vertex buffer object
    // // Generate buffer ID
    // unsigned int vbo;
    // glGenBuffers(1, &vbo);

    // // Bind buffer as a vertex buffer (GL_ARRAY_BUFFER)
    // // and sets the newly created buffer to the GL_ARRAY_BUFFER target
    // glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // // Copy user defined data into a buffer that is currently bound
    // // Takes in the type of buffer that is bound,
    // // the size of data to pass in to the buffer in bytes,
    // // the actual data to send into the the buffer,
    // // and set how the data is managed by the GPU:
    // // GL_STREAM_DRAW: the data is set only once and used by the GPU at most a few times.
    // // GL_STATIC_DRAW: the data is set only once and used many times.
    // // GL_DYNAMIC_DRAW: the data is changed a lot and used many times.
    // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Create a window
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Graphics Engine", NULL, NULL);

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
        // Clear the screen at the start of each frame
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        // Clear only the color buffer for now
        glClear(GL_COLOR_BUFFER_BIT);

        // Process inputs at start of frame
        ProcessInput(window);

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

        // std::cout << timer << std::endl;
        //  Swap buffer that contains render info and outputs it to the screen
        glfwSwapBuffers(window);
        // Check to see if any events are triggered (inputs) and updates the window state
        glfwPollEvents();
    }

    std::cout << "End" << std::endl;

    // Clean and delete all of GLFW's resources that were allocated
    glfwTerminate();

    return 0;
}
