#include "Cube.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "VertexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include <iostream>

Cube::Cube() : RenderObj()
{
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

    mVertexBuffer = new VertexBuffer(vertices, 0, sizeof(vertices), 0, sizeof(vertices) / sizeof(VertexTexture), 0, Vertex::VertexTexture);

    mTextures.emplace_back(new Texture("assets/textures/container.jpg"));
    mTextures.emplace_back(new Texture("assets/textures/awesomeface.png"));
}

Cube::~Cube()
{
    std::cout << "Delete cube" << std::endl;

    delete mVertexBuffer;

    for (auto t : mTextures)
    {
        delete t;
    }
}

void Cube::Update(float deltaTime)
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

void Cube::Draw()
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
