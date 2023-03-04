#include "Engine.h"
#include <iostream>
#include <cmath>
#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include "Texture.h"
#include "VertexFormats.h"
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

    // Enable z-buffering
    glEnable(GL_DEPTH_TEST);

    // // Triangle vertex data
    // float vertices[] = {-0.5f, -0.5f, 0.0f,
    //                     0.5f, -0.5f, 0.0f,
    //                     0.0f, 0.5f, 0.0f};

    // Rectangle vertex data with color attributes and textures
    // VertexColoredTexture vertices[] = {
    //     glm::vec3(0.5f, 0.5f, 0.0f), Color4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f),   // top right, red
    //     glm::vec3(0.5f, -0.5f, 0.0f), Color4(0.0f, 1.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f),  // bottom right, green
    //     glm::vec3(-0.5f, -0.5f, 0.0f), Color4(0.0f, 0.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f), // bottom left, blue
    //     glm::vec3(-0.5f, 0.5f, 0.0f), Color4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)   // top left, white
    // };
    // unsigned int indices[] = {
    //     0, 1, 3, // first triangle
    //     1, 2, 3  // second triangle
    // };

    // Cube
    VertexTexture vertices[] = {glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f),
                                glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(1.0f, 0.0f),
                                glm::vec3(0.5f, 0.5f, -0.5f), glm::vec2(1.0f, 1.0f),
                                glm::vec3(0.5f, 0.5f, -0.5f), glm::vec2(1.0f, 1.0f),
                                glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec2(0.0f, 1.0f),
                                glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f),

                                glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec2(0.0f, 0.0f),
                                glm::vec3(0.5f, -0.5f, 0.5f), glm::vec2(1.0f, 0.0f),
                                glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 1.0f),
                                glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 1.0f),
                                glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec2(0.0f, 1.0f),
                                glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec2(0.0f, 0.0f),

                                glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 0.0f),
                                glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec2(1.0f, 1.0f),
                                glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f),
                                glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f),
                                glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec2(0.0f, 0.0f),
                                glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 0.0f),

                                glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 0.0f),
                                glm::vec3(0.5f, 0.5f, -0.5f), glm::vec2(1.0f, 1.0f),
                                glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f),
                                glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f),
                                glm::vec3(0.5f, -0.5f, 0.5f), glm::vec2(0.0f, 0.0f),
                                glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 0.0f),

                                glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f),
                                glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(1.0f, 1.0f),
                                glm::vec3(0.5f, -0.5f, 0.5f), glm::vec2(1.0f, 0.0f),
                                glm::vec3(0.5f, -0.5f, 0.5f), glm::vec2(1.0f, 0.0f),
                                glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec2(0.0f, 0.0f),
                                glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f),

                                glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec2(0.0f, 1.0f),
                                glm::vec3(0.5f, 0.5f, -0.5f), glm::vec2(1.0f, 1.0f),
                                glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 0.0f),
                                glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 0.0f),
                                glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec2(0.0f, 0.0f),
                                glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec2(0.0f, 1.0f)};

    // Shader
    mShader = new Shader("shaders/texturedVS.glsl", "shaders/texturedFS.glsl");
    mShader->SetActive();

    // Set each sampler to which texture unit it belongs to (only done once)
    mShader->SetInt("textureSampler", 0);
    mShader->SetInt("textureSampler2", 1);

    // Texture
    tex1 = new Texture("assets/textures/container.jpg");
    tex2 = new Texture("assets/textures/awesomeface.png");

    // Vertex buffer
    vBuffer = new VertexBuffer(vertices, nullptr, sizeof(vertices), 0, sizeof(vertices) / sizeof(VertexTexture), 0, Vertex::VertexTexture);

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
    // Model matrix
    // Order in code is Translation, Rotation, Scale
    // even though actual order is Scale, Rotation, Translation
    glm::mat4 model = glm::mat4(1.0f);
    // model = glm::translate(model, glm::vec3(0.3f, 0.1f, 0.0f));
    model = glm::rotate(model, mTimer * glm::radians(50.0f), glm::vec3(0.5, 1.0f, 0.0f));
    // model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));

    // View matrix
    glm::mat4 view = glm::mat4(1.0f);
    // View is 3 units away from origin/target
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    // Projection matrix
    glm::mat4 projection;
    // Create a persepective matrix w/ 45 degree fov
    projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    // Read multiplication right-left
    glm::mat4 viewProj = projection * view;

    // Query the location of uniform in shader:
    // - Supply with shader program
    // - the name of the uniform
    int modelLoc = glGetUniformLocation(mShader->GetID(), "model");
    // glUniform with Matrix4fv as postfix:
    // - Take the uniform's location
    // - The number of matrices to send
    // - Transpose matrix (column-major or row-major)
    //   GLM uses column by default
    // - The actual matrix data converted with glm::value_ptr
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    int viewProjLoc = glGetUniformLocation(mShader->GetID(), "viewProj");
    glUniformMatrix4fv(viewProjLoc, 1, GL_FALSE, glm::value_ptr(viewProj));
}

void Engine::Render()
{
    // Render
    // Clear the screen at the start of each frame
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    // Clear the color/depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw triangles
    // Set a shader program to use
    mShader->SetActive();

    // Bind the texture on their texture units
    glActiveTexture(GL_TEXTURE0);
    tex1->SetActive();
    glActiveTexture(GL_TEXTURE1);
    tex2->SetActive();

    // Draw the vertex buffer
    vBuffer->Draw();

    //  Swap buffer that contains render info and outputs it to the screen
    glfwSwapBuffers(mWindow);
    // Check to see if any events are triggered (inputs) and updates the window state
    glfwPollEvents();
}
