#ifndef DISPLAY_H
#define DISPLAY_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "util/Shader.h"
#include "util/Vector.h"

class Display {
public:
    Display(int, int);
    ~Display();

    bool renderLoop();

private:

    int width, height;

    const float depth = 2;

    const int maxBounceCount = 30;
    const int raysPerPixel = 10;
    
    const float aspectRatio = 2.0 / 2;
    const float viewPortHeight = 2;
    float viewPortWidth;

    Vector3 viewPortU;
    Vector3 viewPortV;

    Vector3 pixelDeltaU;
    Vector3 pixelDeltaV;

    Vector3 viewPortUpperLeft;

    Vector3 pixel00Loc;

    const float verticies[12] = {
        1, 1, 0,
        1, -1, 0,
        -1, -1, 0,
        -1, 1, 0
    };

    const unsigned int indices[6] = {
        0, 1, 3,
        1, 2, 3
    };


    unsigned int VBO, VAO, EBO;

    int vertexPixel00Pos, vertexPixelDeltaU, vertexPixelDeltaV;   

    Shader *shaderProgram;

    GLFWwindow* window;

    void initShaders();

    void calculateDisplaySettings();

};

void framebufferSizeCallback(GLFWwindow*, int, int);

#endif
