#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Shader;

class Engine
{
public:
    Engine();
    ~Engine();

    // Initializes the GLFW library, and any other things required for the engine.
    // Returns true if initialization was a success, false if not.
    bool Init();

    // De-allocates any resources
    void Shutdown();

    // Runs the main game/engine loop
    void Run();

    // Processes any keyboard, mouse, or controller inputs
    // Takes a pointer to a GLFWwindow
    void ProcessInput(GLFWwindow *window);

    // Updates any render objects, components, game logic
    // Takes in a float representing delta time: change in time between two frames
    void Update(float deltaTime);

    // The sets all the buffers, swap chain, textures, vertex array objects for rendering
    void Render();

    // Resizes and adjusts the viewport when the user changes the window size.
    // Takes a pointer to a GLFWwindow and two ints for the new window dimensions.
    // This is called whenever the window changes in size.
    static void FrameBufferSizeCallBack(GLFWwindow *window, int width, int height) { glViewport(0, 0, width, height); }

private:
    // Singleton
    static Engine *s_Engine;

    // The simple shader
    Shader *mShader;

    // Pointer to a GLFWwindow
    GLFWwindow *mWindow;

    // Float to keep track of the time
    float mTimer;

    // Integer to track the amount of frames per second
    unsigned int mFps;

    // Int id for vertex array object
    unsigned int vao = 0;

    // Int ids for textures
    unsigned int texture1 = 0;
    unsigned int texture2 = 0;

    // Bools for toggling between wireframe/fill
    bool mIsWireFrame;
    bool mWirePrev;
};