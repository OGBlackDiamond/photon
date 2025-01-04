#ifndef DISPLAY_H
#define DISPLAY_H

#define balls 1

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "util/Shader.h"
#include "util/Vector.h"
#include "Sphere.h"

class Display {
public:
    Display(int, int);
    ~Display();

    bool renderLoop();

    void addSphere(Sphere);

private:

    int width, height;

    int iteration;

    Sphere *spheres;
    int numSpheres = 0;
    int maxSpheres = 1;

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

    float *sphereMatrixFloats;
    float *sphereVectorFloats;

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

    int vertexPixel00Pos, vertexPixelDeltaU, vertexPixelDeltaV, 
        iterationCount, sphereVectors, sphereFloats, sphereCount;

    Shader *shaderProgram;

    GLFWwindow* window;

    void initShaders();

    void calculateDisplaySettings();

    void initializeSphereInformation();

};

void framebufferSizeCallback(GLFWwindow*, int, int);

#endif
