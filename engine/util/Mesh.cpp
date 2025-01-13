#include "Mesh.h"

Mesh::Mesh(std::string meshPath,
         Vector3 position, Vector3 color, Vector3 emissionColor,
         float emissionStrength, float smoothness) {

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
                
                if (type != "v" && type != "f") {
                    getline(meshFile, a);
                    continue;
                }

                getline(meshFile, a, ' ');
                fa = std::stof(a);

                getline(meshFile, b, ' ');
                fb = std::stof(b);

                getline(meshFile, c);
                fc = std::stof(c);

                Vector3 readVector(fa, fb, fc);

                switch (type.c_str()[0]) {
                    case 'v':
                        addVertex(readVector);
                    case 'f': 
                        addFace(readVector);
                }

            } 
            meshFile.close();
        }


    } catch(std::ifstream::failure e) {
        std::cout << "ERROR::MESH::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        return;
    }


    for (int i = 0; i < numFaces; i++) {
        addTriangle(Triangle(
            position, color, emissionColor,
            emissionStrength, smoothness,
            vertexArray[(int) faceArray[i].x()],
            vertexArray[(int) faceArray[i].y()],
            vertexArray[(int) faceArray[i].z()]
        ));
    }

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

void Mesh::addVertex(Vector3 vertex) {
    if (numVerticies == maxVerticies) {
        maxVerticies *= 2;
        Vector3* tmp = new Vector3[maxVerticies];
        for (int i = 0; i < numVerticies; i++) {
            tmp[i] = vertexArray[i];
        }
        delete[] vertexArray;
        vertexArray = tmp;
    }
    vertexArray[numVerticies] = vertex;
    numVerticies++;
}

void Mesh::addFace(Vector3 face) {
    if (numFaces == maxFaces) {
        maxFaces *= 2;
        Vector3* tmp = new Vector3[maxFaces];
        for (int i = 0; i < numFaces; i++) {
            tmp[i] = faceArray[i];
        }
        delete[] faceArray;
        faceArray = tmp;
    }
    faceArray[numFaces] = face;
    numFaces++;
}
