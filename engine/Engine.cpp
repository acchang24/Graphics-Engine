#include "Engine.h"
#include <iostream>
#include <cmath>
#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include "Texture.h"
#include "VertexBuffer.h"

// Define a window's dimensions
#define WIDTH 800
#define HEIGHT 600

Engine::Engine()
    : mWindow(nullptr), mShader(nullptr), vBuffer(nullptr), tex1(nullptr), tex2(nullptr), mTimer(0.0f), mFps(0), mIsWireFrame(false), mWirePrev(false)
{
}

Engine::~Engine()
{
    Shutdown();
}

bool Engine::Init()
{
    // Initialize the GLFW library
    glfwInit();
    // Support OpenGl Versions 3 to 4
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Use core-profile
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    // Add GLFW_OPENGL_FORWARD_COMPAT for Max OS for the initialization to work
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create a window
    mWindow = glfwCreateWindow(WIDTH, HEIGHT, "Graphics Engine", NULL, NULL);

    // If window creation unsuccessful, cleanup glfw and return false
    if (!mWindow)
    {
        std::cout << "Window creation failed" << std::endl;
        glfwTerminate();
        return false;
    }

    // Set the window's context to the current one
    glfwMakeContextCurrent(mWindow);

    // Initialize GLAD before OpenGl function calls
    // Load the address of the OpenGL function pointers (OS-specific)
    // This gives a glfwGetProcAddress that defines the correct function based on the OS
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    // Create viewport
    // Sets the location of lower left corner (0, 0)
    // Sets width/height of rendering window to the size of GLFW window size
    glViewport(0, 0, WIDTH, HEIGHT);

    // Tell GLFW to call window resize function on every window resize
    // This registers the callback function
    glfwSetFramebufferSizeCallback(mWindow, FrameBufferSizeCallBack);

    // // Triangle vertex data
    // float vertices[] = {-0.5f, -0.5f, 0.0f,
    //                     0.5f, -0.5f, 0.0f,
    //                     0.0f, 0.5f, 0.0f};

    // Rectangle vertex data with color attributes and textures
    float vertices[] = {
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,   // top right, red
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // bottom right, green
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, // bottom left, blue
        -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f   // top left, white
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    // float vertices[] = {-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
    //                     0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
    //                     0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    //                     0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    //                     -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
    //                     -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

    //                     -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    //                     0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
    //                     0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
    //                     0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
    //                     -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
    //                     -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

    //                     -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    //                     -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    //                     -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    //                     -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    //                     -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    //                     -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

    //                     0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    //                     0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    //                     0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    //                     0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    //                     0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    //                     0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

    //                     -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    //                     0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
    //                     0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
    //                     0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
    //                     -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    //                     -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

    //                     -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
    //                     0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    //                     0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    //                     0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    //                     -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
    //                     -0.5f, 0.5f, -0.5f, 0.0f, 1.0f};

    // Shader
    mShader = new Shader("shaders/simpleVS.glsl", "shaders/simpleFS.glsl");
    mShader->SetActive();

    // Set each sampler to which texture unit it belongs to (only done once)
    mShader->SetInt("textureSampler", 0);
    mShader->SetInt("textureSampler2", 1);

    // Texture
    tex1 = new Texture("assets/textures/container.jpg");
    tex2 = new Texture("assets/textures/awesomeface.png");

    vBuffer = new VertexBuffer(vertices, indices, sizeof(vertices), sizeof(indices), sizeof(float), sizeof(unsigned int));

    return true;
}

void Engine::Shutdown()
{
    std::cout << "SHUTDOWN" << std::endl;
    if (mShader)
    {
        delete mShader;
    }
    if (tex1)
    {
        delete tex1;
    }
    if (tex2)
    {
        delete tex2;
    }
    if (vBuffer)
    {
        delete vBuffer;
    }
    // Clean and delete all of GLFW's resources that were allocated
    glfwTerminate();
}

void Engine::Run()
{
    // Get a timestamp of the current time
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

    // Render loop
    // glfwWindowShouldClose() checks at the start of each
    // loop iteration to see if GLFW needs to be closed
    while (!glfwWindowShouldClose(mWindow))
    {
        // Process inputs at start of frame
        ProcessInput(mWindow);

        // Toggle wireframe draws
        if (mIsWireFrame)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

        // Get a time stamp of the current time and set that as the end time
        std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
        // Get the duration of the two time stamps
        double duration = static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count());
        // Cast to float and calculate the delta time
        float deltaTime = static_cast<float>(0.000000001 * duration);
        // Set the new starting time stamp to the current end time stamp
        start = end;

        Update(deltaTime);

        mFps = static_cast<int>(1.0f / deltaTime);

        // Increment the timer by deltaTime
        mTimer += deltaTime;

        Render();
    }
}

void Engine::ProcessInput(GLFWwindow *window)
{
    // If the user presses the escape key, close the window
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        // Set glfwSetWindowShouldClose to true
        glfwSetWindowShouldClose(window, true);
    }

    // Toggles wireframe drawing
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && !mWirePrev)
    {
        mWirePrev = true;
        mIsWireFrame = !mIsWireFrame;
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_RELEASE && mWirePrev)
    {
        mWirePrev = false;
    }
}

void Engine::Update(float deltaTime)
{
    // Identity matrix
    glm::mat4 trans = glm::mat4(1.0f);

    // Order in code is Translation, Rotation, Scale
    // even though actual order is Scale, Rotation, Translation

    // Translation
    // trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));

    // Rotation
    // Rotate positive angle on z axis
    trans = glm::rotate(trans, mTimer * 2, glm::vec3(0.0, 0.0, 1.0));

    // Scale
    // Scale by 0.5
    // trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));

    // Query the location of uniform in shader:
    // - Supply with shader program
    // - the name of the uniform
    unsigned int transformLoc = glGetUniformLocation(mShader->GetID(), "transform");

    // glUniform with Matrix4fv as postfix:
    // - Take the uniform's location
    // - The number of matrices to send
    // - Transpose matrix (column-major or row-major)
    //   GLM uses column by default
    // - The actual matrix data converted with glm::value_ptr
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

    // Model matrix
    glm::mat4 model = glm::mat4(1.0f);
    // Rotate along x axis to make it look like a plane
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    // View matrix
    glm::mat4 view = glm::mat4(1.0f);
    // View is 3 units away from origin/target
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    // Projection matrix
    glm::mat4 projection;
    // Create a persepective matrix w/ 45 degree fov
    projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    glm::mat4 viewProj = projection * view;

    int modelLoc = glGetUniformLocation(mShader->GetID(), "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    int viewProjLoc = glGetUniformLocation(mShader->GetID(), "viewProj");
    glUniformMatrix4fv(viewProjLoc, 1, GL_FALSE, glm::value_ptr(viewProj));
}

void Engine::Render()
{
    // Render
    // Clear the screen at the start of each frame
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    // Clear only the color buffer for now
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw triangles
    // Set a shader program to use
    mShader->SetActive();

    // // Vary the color in range of 0.0f-1.0f using sin function
    // float greenValue = (sinf(mTimer) / 2.0f) + 0.5f;
    // // Query the location of uniform in shader:
    // // - Supply with shader program
    // // - the name of the uniform
    // int vertexColorLocation = glGetUniformLocation(mShader->GetID(), "changeColor");

    // // Set the uniform value using glUniform4f. Must be done after setting an active shader program to use
    // glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

    // Bind the texture on their texture units
    glActiveTexture(GL_TEXTURE0);
    tex1->SetActive();
    glActiveTexture(GL_TEXTURE1);
    tex2->SetActive();

    // Draw the vertex buffer
    vBuffer->Draw();

    // // // glDrawArrays to draw primitives using the active shader:
    // // // - First argument takes the OpenGL primitive type to draw. In this case, draw triangles
    // // // - Second argument specifies the starting index of the vertex array to draw
    // // // - Last argument specifies how many vertices to draw, in this case it is 3
    // // glDrawArrays(GL_TRIANGLES, 0, 3);

    // //   glDrawElements takes indices from EBO currently bound to GL_ELEMENT_ARRAY_BUFFER target:
    // // - First argument specifies the mode to draw in, in this case still triangles
    // // - Second argument is the count or number of elements to draw
    // // - Third argument is type of indices which is of GL_UNSIGNED_INT
    // // - Last argument allows us to specify offset in EBO or pass in an index array
    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // No need to unbind every time
    // glBindVertexArray(0);

    //  Swap buffer that contains render info and outputs it to the screen
    glfwSwapBuffers(mWindow);
    // Check to see if any events are triggered (inputs) and updates the window state
    glfwPollEvents();
}
