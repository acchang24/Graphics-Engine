#pragma once
#include <glad/glad.h>
#include <cstdlib>
#include "VertexFormats.h"

// The VertexBuffer class takes in all the vertex and index information
// of an object and creates a vertex array object. Takes in a const void *
// for the vertex/index data, as well as the size in bytes the vertex and index arrays.
// It also takes in the the size in bytes of the data type for the vertex/index.
// This class will save them as buffers with an unsigned int for its ID.
class VertexBuffer
{
public:
    VertexBuffer(const void *vertices, const void *indices, size_t vertexSize, size_t indexSize, size_t vertexCount, size_t indexCount, Vertex vertexFormat);
    ~VertexBuffer();

    // Sets all the vertex attributes and link the 
    // Vertex Attributes with glVertexAttribPointer():
    void SetVertexAttributePointers(Vertex format);

    // Bind the Vertex Array Object
    void SetActive() { glBindVertexArray(mVaoID); }

    // Getter for the VAO id
    unsigned int GetID() const { return mVaoID; }

    // Sets the VAO as active, and draws based on
    // if it is index or not
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