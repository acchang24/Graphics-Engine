#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Test.h"

#define WIDTH 1280
#define HEIGHT 720

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
    // Initialize the GLFW library
    if (!glfwInit())
    {
        std::cout << "GLFW initialization failed" << std::endl;
        return -1;
    }
    else
    {
        // OpenGl Version 3
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        // Use core-profile
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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
        // glfwWindowShouldClose checks at the start of each loop iteration
        // to see if GLFW needs to be closed
        while (!glfwWindowShouldClose(window))
        {
            // Clear the screen
            glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
            // Clear only the color buffer for now
            glClear(GL_COLOR_BUFFER_BIT);

            // Process any input at the start of each frame
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

            std::cout << timer << std::endl;
            // Swap buffer that contains render info and outputs it to the screen
            glfwSwapBuffers(window);
            // Check to see if any events are triggered (inputs) and updates the window state
            glfwPollEvents();
        }
    }

    Test testing;
    for (int i = 0; i < 10; ++i)
    {
        testing.IncrementValue();
    }

    std::cout << testing.GetValue() << std::endl;

    // Clean and delete all of GLFW's resources that were allocated
    glfwTerminate();

    return 0;
}