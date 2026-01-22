#include "Mesh.h"

Mesh::Mesh(std::string meshPath,
         glm::vec3 position, Surface surface) {

    triArray = new Triangle[maxTris];
    vertexArray = new glm::vec3[maxVerticies];
    faceArray = new glm::vec3[maxFaces];

    try {

        std::fstream meshFile(
            meshPath,
            std::ios::in
        );

        if (meshFile) {
            while (!meshFile.eof()) {

                std::string type, a, b, c;

                float fa, fb, fc;

                 getline(meshFile, type, ' ');

                 if (type == "v") {
                     getline(meshFile, a, ' ');
                     fa = std::stof(a);

                     getline(meshFile, b, ' ');
                     fb = std::stof(b);

                     getline(meshFile, c);
                     fc = std::stof(c);

                     glm::vec3 readVector(fa, fb, fc);
                     addVertex(readVector);
                 } else if (type == "f") {
                     std::string faceLine;
                     getline(meshFile, faceLine);
                     std::stringstream ss(faceLine);
                     std::string token;
                     std::vector<int> indices;
                     while (std::getline(ss, token, ' ')) {
                         if (!token.empty()) {
                             std::stringstream ss2(token);
                             std::string vertexIndex;
                             std::getline(ss2, vertexIndex, '/');
                             indices.push_back(std::stoi(vertexIndex));
                         }
                     }
                     if (indices.size() >= 3) {
                         glm::vec3 readVector(indices[0], indices[1], indices[2]);
                         addFace(readVector);
                     }
                 } else {
                     getline(meshFile, a);
                     continue;
                 }

            } 
            meshFile.close();
        }



    } catch(std::ifstream::failure e) {
        std::cout << "ERROR::MESH::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        return;
    }

    this->position = position;
    this->color = surface.surfaceColor;
    this->emissionColor = surface.emissionColor;

    this->emissionStrength = surface.emissionStrength;
    this->smoothness = surface.smoothness;


    for (int i = 0; i < numFaces; i++) {
        addTriangle({
            position, 
            vertexArray[(int)(faceArray[i].x - 1)],
            vertexArray[(int)(faceArray[i].y - 1)],
            vertexArray[(int)(faceArray[i].z - 1)],
            {
                color, emissionColor,
                emissionStrength, smoothness
            }
        });

    }

    std::cout << numTris << std::endl;
    std::cout << numFaces << std::endl;
}

Mesh::~Mesh() {
    delete[] triArray;
    delete[] vertexArray;
    delete[] faceArray;
}

void Mesh::addTriangle(Triangle tri) {
    if (numTris == maxTris) {
        maxTris *= 2;
        Triangle* tmp = new Triangle[maxTris];
        for (int i = 0; i < numTris; i++) {
            tmp[i] = triArray[i];
        }
        delete[] triArray;
        triArray = tmp;
    }
    triArray[numTris] = tri;
    numTris++;
}

void Mesh::addVertex(glm::vec3 vertex) {
    if (numVerticies == maxVerticies) {
        maxVerticies *= 2;
        glm::vec3* tmp = new glm::vec3[maxVerticies];
        for (int i = 0; i < numVerticies; i++) {
            tmp[i] = vertexArray[i];
        }
        delete[] vertexArray;
        vertexArray = tmp;
    }
    vertexArray[numVerticies] = vertex;
    numVerticies++;
}

void Mesh::addFace(glm::vec3 face) {
    if (numFaces == maxFaces) {
        maxFaces *= 2;
        glm::vec3* tmp = new glm::vec3[maxFaces];
        for (int i = 0; i < numFaces; i++) {
            tmp[i] = faceArray[i];
        }
        delete[] faceArray;
        faceArray = tmp;
    }
    faceArray[numFaces] = face;
    numFaces++;
}
