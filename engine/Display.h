#ifndef DISPLAY_H
#define DISPLAY_H


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "util/Shader.h"
#include "util/Sphere.h"
#include "util/Triangle.h"
#include "util/Mesh.h"


class Display {
public:
    Display(int, int);
    ~Display();

    bool renderLoop();

    void addSphere(Sphere);

    void setMesh(Mesh*);

    void initSSBO();

private:

    int width, height;

    int iteration;

    Sphere* spheres;
    int numSpheres = 0;
    int maxSpheres = 1;

    Mesh* mesh;

    const float depth = 2;

    const int maxBounceCount = 30;
    const int raysPerPixel = 10;
    
    const float aspectRatio = 2.0 / 2;
    const float viewPortHeight = 2;
    float viewPortWidth;

    glm::vec3 viewPortU;
    glm::vec3 viewPortV;

    glm::vec3 pixelDeltaU;
    glm::vec3 pixelDeltaV;

    glm::vec3 viewPortUpperLeft;

    glm::vec3 pixel00Loc;


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


    unsigned int VBO, VAO, EBO, sphereSSBO, triangleSSBO;

    GLuint fbo[2];
    GLuint tex[2];

    int vertexPixel00Pos, vertexPixelDeltaU, vertexPixelDeltaV,
        sphereCount, triCount;

    GLint frameCountLoc, isAccumulatingLoc, prevTexLoc, accumTexLoc, screenWidthLoc, screenHeightLoc;

    Shader* shaderProgram;

    GLFWwindow* window;

    void initShaders();

    void calculateDisplaySettings();

};

void framebufferSizeCallback(GLFWwindow*, int, int);

#endif
