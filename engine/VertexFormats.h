#pragma once
#include <glm/glm.hpp>
#include <vector>

// Enum to store different vertex layouts
enum class Vertex
{
    VertexColor,
    VertexTexture,
    VertexColoredTexture,
};

// GetVertexFormat statically returns the stride or spacing between each attribute of the vertex.
// It returns a vector of ints that represents the number of values between each attribute
static std::vector<int> GetVertexFormat(Vertex vertex)
{
    std::vector<int> format;
    switch (vertex)
    {
    case Vertex::VertexColor:
        format.emplace_back(3);
        format.emplace_back(4);
        break;
    case Vertex::VertexTexture:
        format.emplace_back(3);
        format.emplace_back(2);
        break;
    case Vertex::VertexColoredTexture:
        format.emplace_back(3);
        format.emplace_back(4);
        format.emplace_back(2);
    }
    return format;
}

// Class to define a color with 4 channels
class Color4
{
public:
    Color4()
        : r(0), g(0), b(0), a(0)
    {
    }
    Color4(float _r, float _g, float _b, float _a = 1.0f)
        : r(_r), g(_g), b(_b), a(_a)
    {
    }
    float r, g, b, a;
};

struct VertexColor
{
    glm::vec3 pos;
    Color4 color;
};

struct VertexTexture
{
    glm::vec3 pos;
    glm::vec2 uv;
};

struct VertexColoredTexture
{
    glm::vec3 pos;
    Color4 color;
    glm::vec2 uv;
};
