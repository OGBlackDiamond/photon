#ifndef MESH_H 
#define MESH_H

#include <string>
#include <iostream>
#include <fstream>

#include "Surface.h"
#include "Triangle.h"
#include <glm/glm.hpp>

class Mesh {
public:
    Mesh() {}

    Mesh(std::string meshPath,
         glm::vec3 position, Surface surface);

    ~Mesh();

    int numTris = 0, numVerticies = 0, numFaces = 0;

    Triangle* triArray;
    glm::vec3* vertexArray;
    glm::vec3* faceArray;

    glm::vec3 position, color, emissionColor;
    float emissionStrength, smoothness;

private:
    int maxTris = 1, maxVerticies = 1, maxFaces = 1;

    void addVertex(glm::vec3);
    void addFace(glm::vec3);
    void addTriangle(Triangle);
};

#endif
