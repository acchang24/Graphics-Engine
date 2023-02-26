#include "Shader.h"
#include <iostream>

Shader::Shader(const std::string &vertexFile, const std::string &fragmentFile)
{
    // Strings to hold the vertex/fragment codes
    std::string vertexCode;
    std::string fragmentCode;

    // Open the files and read them into the strings
    std::ifstream vsFile(vertexFile);
    std::ifstream fsFile(fragmentFile);

    if (vsFile.is_open() && fsFile.is_open())
    {
        std::string line;

        while (std::getline(vsFile, line))
        {
            vertexCode += line + "\n";
        }
        while (std::getline(fsFile, line))
        {
            fragmentCode += line + "\n";
        }
        vsFile.close();
        fsFile.close();

        // Compile the shaders
        CompileShaders(vertexCode.c_str(), fragmentCode.c_str());
    }
    else
    {
        std::cout << "Can't open shader files" << std::endl;
    }
}

Shader::~Shader()
{
    // Delete the shader program
    glDeleteProgram(mShaderID);
    mShaderID = 0;
}

void Shader::CompileShaders(const char *vertexCode, const char *fragmentCode)
{
    // Vertex shader
    // Create a vertex shader object with glCreateShader and store its id as an int
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //   Attach shader code to the shader object:
    // - Takes the shader object to compile,
    // - How many strings that are going to be passed in as source code,
    // - The actual source code of the shader,
    // - An array of string lengths
    glShaderSource(vertexShader, 1, &vertexCode, NULL);
    // Compile the shader
    glCompileShader(vertexShader);

    // Check to see if shader compilation was successful:
    // Define an integer to indicate success
    int success = 0;
    // Char array for any error messages
    char infoLog[512];
    // Get any errors
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "Vertex shader compilation failed\n"
                  << infoLog << std::endl;
    }

    // Fragment shader (similar to vertex shader)
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentCode, NULL);
    glCompileShader(fragmentShader);
    int success1 = 0;
    char infoLog1[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success1);
    if (!success1)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog1);
        std::cout << "Fragment shader compilation failed\n"
                  << infoLog1 << std::endl;
    }

    // Shader program
    // Create a shader program and return an ID reference
    mShaderID = glCreateProgram();
    // Attatch the compiled vertex/fragment shaders to the program object
    glAttachShader(mShaderID, vertexShader);
    glAttachShader(mShaderID, fragmentShader);
    // Link to shader program
    glLinkProgram(mShaderID);
    // Check to see if program failed and retrieve the log
    int success2 = 0;
    char infoLog2[512];
    glGetProgramiv(mShaderID, GL_LINK_STATUS, &success2);
    if (!success)
    {
        glGetProgramInfoLog(mShaderID, 512, NULL, infoLog2);
        std::cout << "Shader program creation failed\n"
                  << infoLog2 << std::endl;
    }

    // Delete the shaders once they have been linked to the program
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}
