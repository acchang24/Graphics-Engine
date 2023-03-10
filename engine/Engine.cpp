#include "Engine.h"
#include <iostream>
#include <cmath>
#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "AssetManager.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexFormats.h"
#include "VertexBuffer.h"
#include "RenderObj.h"
#include "Cube.h"

// Define a window's dimensions
#define WIDTH 1280
#define HEIGHT 720

Engine::Engine()
    : mWindow(nullptr), mAssetManager(nullptr), // vBuffer(nullptr),
      mTimer(0.0f), mFps(0), mIsWireFrame(false), mWirePrev(false)
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

    // VertexTexture vertices[] = {
    //     glm::vec3(0.5f, 0.5f, 0.0f), glm::vec2(1.0f, 1.0f),   // top right
    //     glm::vec3(0.5f, -0.5f, 0.0f), glm::vec2(1.0f, 0.0f),  // bottom right
    //     glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec2(0.0f, 0.0f), // bottom left
    //     glm::vec3(-0.5f, 0.5f, 0.0f), glm::vec2(0.0f, 1.0f)   // top left
    // };
    // unsigned int indices[] = {
    //     0, 1, 3, // first triangle
    //     1, 2, 3  // second triangle
    // };

    // AssetManager
    mAssetManager = new AssetManager();

    // Shader
    Shader *mShader = new Shader("shaders/texturedVS.glsl", "shaders/texturedFS.glsl");
    mShader->SetActive();

    // Set each sampler to which texture unit it belongs to(only done once)
    mShader->SetInt("textureSampler", 0);
    mShader->SetInt("textureSampler2", 1);

    // Cache/save the shader
    mAssetManager->SaveShader("textured", mShader);

    // Create Textures
    Texture *tex1 = new Texture("assets/textures/container.jpg");
    Texture *tex2 = new Texture("assets/textures/awesomeface.png");

    // Vertex buffer
    // vBuffer = new VertexBuffer(vertices, indices, sizeof(vertices), sizeof(indices), sizeof(vertices) / sizeof(VertexTexture), sizeof(indices) / sizeof(unsigned int), Vertex::VertexTexture);

    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)};

    for (int i = 0; i < 10; ++i)
    {
        Cube *cube = new Cube();
        cube->SetPosition(cubePositions[i]);
        cube->SetShader(mShader);
        mObjects.emplace_back(cube);
    }

    return true;
}

void Engine::Shutdown()
{
    std::cout << "SHUTDOWN" << std::endl;

    delete mAssetManager;

    // Delete all objects
    for (auto o : mObjects)
    {
        delete o;
    }
    mObjects.clear();

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
    // Update the object
    for (auto o : mObjects)
    {
        o->Update(deltaTime);
    }

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

    // Get location of viewProj in shader
    int viewProjLoc = glGetUniformLocation(mAssetManager->Get()->LoadShader("textured")->GetID(), "viewProj");
    // Update viewProj
    glUniformMatrix4fv(viewProjLoc, 1, GL_FALSE, glm::value_ptr(viewProj));
}

void Engine::Render()
{
    // Render
    // Clear the screen at the start of each frame
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    // Clear the color/depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Loop through and draw all the objects
    for (auto o : mObjects)
    {
        o->Draw();
    }

    //  Swap buffer that contains render info and outputs it to the screen
    glfwSwapBuffers(mWindow);
    // Check to see if any events are triggered (inputs) and updates the window state
    glfwPollEvents();
}
