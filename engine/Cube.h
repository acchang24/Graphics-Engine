#pragma once
#include "RenderObj.h"

// Cube Primitive: creates a 3D cube
class Cube : public RenderObj
{
public:
    Cube();
    ~Cube();

    void Update(float deltaTime) override;
    void Draw() override;

private:
};