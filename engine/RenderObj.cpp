#include "RenderObj.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "VertexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include <iostream>

RenderObj::RenderObj()
    : mVertexBuffer(nullptr), mShader(nullptr), mModel(glm::mat4(1.0f)), mPosition(glm::vec3(0.0f, 0.0f, 0.0f)), mScale(glm::vec3(1.0f, 1.0f, 1.0f)), mTimer(0.0f)
{
}

RenderObj::RenderObj(VertexBuffer *vBuffer, Shader *shader, const std::vector<Texture *> &textures)
    : mVertexBuffer(vBuffer), mShader(shader), mTextures(textures), mModel(glm::mat4(1.0f)), mPosition(glm::vec3(0.0f, 0.0f, 0.0f)), mScale(glm::vec3(1.0f, 1.0f, 1.0f)), mTimer(0.0f)
{
}

RenderObj::~RenderObj()
{
    std::cout << "Delete render object" << std::endl;
}

void RenderObj::Update(float deltaTime)
{
    // Update model matrix
    mModel = glm::mat4(1.0f);

    //////// Update timer ////////
    mTimer += deltaTime;

    // Translate
    mModel = glm::translate(mModel, mPosition);

    // Rotate on x/y axis
    mModel = glm::rotate(mModel, mTimer * glm::radians(50.0f), glm::vec3(0.5, 1.0f, 0.0f));

    // Scale
    mModel = glm::scale(mModel, mScale);
}

void RenderObj::Draw()
{
    // Set a shader program to use
    mShader->SetActive();

    // Bind the texture on their texture units
    for (size_t i = 0; i < mTextures.size(); ++i)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        mTextures[i]->SetActive();
    }

    //   Query the location of uniform containing the model matrix in shader:
    // - Supply with shader program
    // - the name of the uniform
    int modelLoc = glGetUniformLocation(mShader->GetID(), "model");

    //   Send model matrix to GPU using glUniform with Matrix4fv as postfix:
    // - Take the uniform's location
    // - The number of matrices to send
    // - Transpose matrix (column-major or row-major). GLM uses column by default (GL_FALSE)
    // - The actual matrix data converted with glm::value_ptr
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mModel));

    // Draw the vertex buffer
    mVertexBuffer->Draw();
}
