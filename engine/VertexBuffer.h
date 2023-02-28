#pragma once
#include <glad/glad.h>

// The VertexBuffer class takes in all the vertex and index information
// of an object and creates a vertex array object. Takes in a void *
// as well as the byte size for both the vertices the indices.
// This class will save them as buffers with an unsigned int for its ID.
class VertexBuffer
{
public:
    VertexBuffer(const void *vertices, const void *indices, size_t vertexByteSize, size_t indexByteSize);
    ~VertexBuffer();

    void SetActive() { glBindVertexArray(mVaoID); }

    unsigned int GetID() { return mVaoID; }

private:
    // ID for the Vertex Array Object
    unsigned int mVaoID;

    // ID for the vertex buffer
    unsigned int mVertexBufferID;

    // ID for the index buffer
    unsigned mIndexBufferID;
};