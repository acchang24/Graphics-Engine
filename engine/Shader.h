#pragma once
#include <string>
#include <glad/glad.h>

// Shader class contains a OpenGL shader program that consists of
// a vertex shader and a fragment shader. This shader class manages
// when a particular shader program is being set as active, as well as
// functions to help set any uniforms set by its shaders.
class Shader
{
public:
    //   Shader constructor:
    // - const std::string& for the vertex shader name/file path
    // - const std::string& for the fragment shader name/file path
    Shader(const std::string &vertexFile, const std::string &fragmentFile);
    ~Shader();

    //   CompileShaders compiles both the vertex and fragment shaders and links them into a program.
    // - Takes in 2 const char* of the vertex and fragment codes as parameters
    void CompileShaders(const char *vertexCode, const char *fragmentCode);

    // Sets this shader program as the active one with glUseProgram
    // Every shader/rendering call will use this program object and its shaders
    void SetActive() { glUseProgram(mShaderID); }

    // Getter for the shader program's id
    int GetID() const { return mShaderID; }

    // Setters for bool, int, and float uniforms
    void SetBool(const std::string &name, bool value) const
    {
        glUniform1i(glGetUniformLocation(mShaderID, name.c_str()), static_cast<int>(value));
    }

    void SetInt(const std::string &name, int value) const
    {
        glUniform1i(glGetUniformLocation(mShaderID, name.c_str()), value);
    }

    void SetFloat(const std::string &name, float value) const
    {
        glUniform1f(glGetUniformLocation(mShaderID, name.c_str()), value);
    }

private:
    // The shader's ID
    unsigned int mShaderID;
};