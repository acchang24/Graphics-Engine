#pragma once
#include <vector>
#include <glm/glm.hpp>

class VertexBuffer;
class Shader;
class Texture;

// The RenderObj class describes any drawable object for the engine.
// It controls how each object is updated and drawn on each frame.
// RenderObjects wrap everything such as the VertexBuffer, Shaders,
// and Textures into a single object that makes respective calls
// during the RenderObj's Update and Draw functions. RenderObj
// is also responsible for the object's own model matrix, which transforms
// the object from model space into world space as it updates.
class RenderObj
{
public:
    RenderObj();
    //   RenderObj constructor:
    // - VertexBuffer* to handle the object's VAO, vertex/index buffer
    // - Shader* to handle how the object will look
    // - const std::vector<Texture*>& for the Object's textures
    RenderObj(VertexBuffer *vBuffer, Shader *shader, const std::vector<Texture *> &textures);
    virtual ~RenderObj();

    // Updates the RenderObj
    virtual void Update(float deltaTime);

    // Draws the RenderObj
    virtual void Draw();

    // Setters for Shader and Textures
    void SetShader(Shader *shader) { mShader = shader; }
    void AddTexture(Texture *texture) { mTextures.emplace_back(texture); }

    // Getters for the RenderObj's model matrix, position, and scale
    glm::mat4 &GetModelMatrix() { return mModel; }
    glm::vec3 &GetPosition() { return mPosition; }
    glm::vec3 &GetScale() { return mScale; }

    // Setters for the RenderObj's model matrix, position, and scale
    void SetModelMatrix(const glm::mat4 &model) { mModel = model; }
    void SetPosition(const glm::vec3 &pos) { mPosition = pos; }
    void SetScale(const glm::vec3 &scale) { mScale = scale; }

protected:
    // Object's vertex buffer
    VertexBuffer *mVertexBuffer;

    // Shader the object uses
    Shader *mShader;

    // Vector of textures
    std::vector<Texture *> mTextures;

    // The object's model matrix
    glm::mat4 mModel;

    // Vector3 for position/scale
    glm::vec3 mPosition;
    glm::vec3 mScale;

    //// TEMP TIMER
    float mTimer;
};
