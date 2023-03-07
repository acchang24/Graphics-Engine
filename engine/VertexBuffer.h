#pragma once
#include <glad/glad.h>
#include <cstdlib>
#include "VertexFormats.h"

// The VertexBuffer class takes in all the vertex and index information
// of an object and creates an OpenGL Vertex Array Object. This class will save
// the VAO and the vertex/index buffers with an unsigned int that can be referenced
// later with its ID.
class VertexBuffer
{
public:
    //   Constructor of VertexBuffer:
    // - 2 const void* for the actual vertex/index data
    // - 2 size_t for the size in bytes of the vertex/index arrays
    // - 2 size_t for the number of vertices and indices
    // - enum class Vertex for the format of the vertex
    VertexBuffer(const void *vertices, const void *indices, size_t vertexSize, size_t indexSize, size_t vertexCount, size_t indexCount, Vertex vertexFormat);
    ~VertexBuffer();

    //   SetVertexAttributePointers sets all the vertex attributes
    //   and links the Vertex Attributes with glVertexAttribPointer:
    // - Takes in an enum class of Vertex that represents the format of the vertex
    void SetVertexAttributePointers(Vertex format);

    // Bind the Vertex Array Object using glBindVertexArray with mVaoID as its parameter
    void SetActive() { glBindVertexArray(mVaoID); }

    // Getter for the Vertex Array's ID
    unsigned int GetID() const { return mVaoID; }

    // Sets the VAO as active, and draws based on if it is drawn with indices or not
    void Draw();

private:
    // ID for the Vertex Array Object
    unsigned int mVaoID;

    // ID for the vertex / index buffer
    unsigned int mVertexBufferID;

    unsigned int mIndexBufferID;

    // Vertex and index count
    size_t mVertexCount;

    size_t mIndexCount;

    // bool for if the VA draws w/ indices
    bool mDrawIndexed;
};