#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

class Shader;
class Texture;
class VertexBuffer;
class RenderObj;

// The main Engine class that controls the graphics. This class
// drives the input processing of any controllers/mouse/keyboard inputs,
// updating, and rendering objects to the screen. It contains the main
// render/game loop of the application.
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
    // Pointer to a GLFWwindow
    GLFWwindow *mWindow;

    // The simple shader
    Shader *mShader;

    //VertexBuffer *vBuffer;

    // Textures
    //Texture *tex1;
    //Texture *tex2;

    std::vector<RenderObj *> mObjects;

    // Float to keep track of the time
    float mTimer;

    // Integer to track the amount of frames per second
    unsigned int mFps;

    // Bools for toggling between wireframe/fill
    bool mIsWireFrame;
    bool mWirePrev;
};