#include "VertexBuffer.h"
#include <iostream>

VertexBuffer::VertexBuffer(const void *vertices, const void *indices, size_t vertexSize, size_t indexSize, size_t vertexStride, size_t indexStride)
    : mVaoID(0), mVertexBufferID(0), mIndexBufferID(0), mVertexCount(vertexSize / vertexStride), mIndexCount(indexSize / indexStride), mDrawIndexed(false)
{
    // Create a vertex array object, store in int as reference
    glGenVertexArrays(1, &mVaoID);
    // Bind the the new vertex array object with glBindVertexArray first,
    // then set any vertex buffers, vertex attributes, and any index buffers
    glBindVertexArray(mVaoID);

    // Generate a new buffer for vertex buffer
    glGenBuffers(1, &mVertexBufferID);
    // Bind the new buffer as a vertex buffer (GL_ARRAY_BUFFER)
    // and sets the newly created buffer to the GL_ARRAY_BUFFER target
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);

    // Copy user defined data into a buffer that is currently bound
    // Takes in the type of buffer that is bound,
    // the size of data to pass in to the buffer in bytes,
    // the actual data to send into the the buffer,
    // and set how the data is managed by the GPU:
    // GL_STREAM_DRAW: the data is set only once and used by the GPU at most a few times.
    // GL_STATIC_DRAW: the data is set only once and used many times.
    // GL_DYNAMIC_DRAW: the data is changed a lot and used many times.
    glBufferData(GL_ARRAY_BUFFER, vertexSize, vertices, GL_STATIC_DRAW);

    if (indices)
    {
        mDrawIndexed = true;

        // Genrate a new buffer for index buffer (Element buffer object)
        glGenBuffers(1, &mIndexBufferID);

        // Copy index array in a element buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize, indices, GL_STATIC_DRAW);
    }

    // Set vertex attributes pointers
    //   Link Vertex Attributes with glVertexAttribPointer():
    // - First argument specifies which vertex attribute to configure.
    //   Specified the plocation of position vertex attribute with layout (location = 0) in the vertex shader
    //   This sets location of vertex attribute to 0, and so pass in 0 for the data of this vertex attribute
    // - Second argument specifies size of the vertex attribute. This attribute is vec3, so it takes 3 values
    // - Third argument specifies the type of the data, which in this case is a GL_Float (vec* in GLSL)
    // - Fourth argument specifies if the data is going to be normalized.
    // - Fifth argument is the stride, and defines the space between consecutive vertex attributes
    // - Last argument is type void*, and is the offset of where the position data begins in the buffer
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *)0);
    // Enable vertex attribute, giving the vertex attribute location as its argument
    glEnableVertexAttribArray(0);

    // Color attribute
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *)(7 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

VertexBuffer::~VertexBuffer()
{
    std::cout << "Delete vertex arrays and vertex/index buffers" << std::endl;
    // De-allocate all resources
    glDeleteVertexArrays(1, &mVaoID);
    glDeleteBuffers(1, &mVertexBufferID);
    glDeleteBuffers(1, &mIndexBufferID);

    mVaoID = 0;
    mVertexBufferID = 0;
    mIndexBufferID = 0;
}

void VertexBuffer::Draw()
{
    // First bind the vertex array
    SetActive();

    if (mDrawIndexed)
    {
        //   glDrawElements takes indices from EBO currently bound to GL_ELEMENT_ARRAY_BUFFER target:
        // - First argument specifies the mode to draw in, in this case still triangles
        // - Second argument is the count or number of elements to draw
        // - Third argument is type of indices which is of GL_UNSIGNED_INT
        // - Last argument allows us to specify offset in EBO or pass in an index array
        glDrawElements(GL_TRIANGLES, mIndexCount, GL_UNSIGNED_INT, 0);
    }
    else
    {
        // glDrawArrays to draw primitives using the active shader:
        // - First argument takes the OpenGL primitive type to draw. In this case, draw triangles
        // - Second argument specifies the starting index of the vertex array to draw
        // - Last argument specifies how many vertices to draw, in this case it is 3
        glDrawArrays(GL_TRIANGLES, 0, mVertexCount);
    }
}
