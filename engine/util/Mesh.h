#ifndef MESH_H 
#define MESH_H

#include "Triangle.h"

#include <string>
#include <iostream>
#include <fstream>

class Mesh {
public:
    Mesh(std::string meshPath,
         Vector3 position, Vector3 color, Vector3 emissionColor,
         float emissionStrength, float smoothness);

    ~Mesh();

private:

    Triangle* triArray;
    Vector3* vertexArray;
    Vector3* faceArray;

    int numTris = 0, numVerticies = 0, numFaces = 0;
    int maxTris = 1, maxVerticies = 1, maxFaces = 1;

    Vector3 position, color, emissionColor;
    float emissionStrength, smoothness;

    void addVertex(Vector3);
    void addFace(Vector3);
    void addTriangle(Triangle);

};

#endif
