#ifndef TRIANGLE_H 
#define TRIANGLE_H

#include "Vector.h"

struct Triangle{

    Vector3 position;
    Vector3 color;
    Vector3 emissionColor;
    float emissionStrength;
    float smoothness;

    Vector3 v1, v2, v3;


    Triangle() {
        position = Vector3();
        color = Vector3();
        emissionColor = Vector3();
        emissionStrength = -1;
        smoothness = -1;

        v1 = Vector3();
        v2 = Vector3();
        v3 = Vector3();
    }

    Triangle(Vector3 position, Vector3 color, Vector3 emissionColor,
            float emissionStrength, float smoothness,
            Vector3 v1, Vector3 v2, Vector3 v3) {
        this->position = position;
        this->color = color;
        this->emissionColor = emissionColor;
        this->emissionStrength = emissionStrength;
        this->smoothness = smoothness;

        this->v1 = v1;
        this->v2 = v2;
        this->v3 = v3;
    }

    Triangle& operator=(const Triangle& rhs) {
        position = rhs.position;
        color = rhs.color;
        emissionColor = rhs.emissionColor;
        emissionStrength = rhs.emissionStrength;
        smoothness = rhs.smoothness;

        v1 = rhs.v1;
        v2 = rhs.v2;
        v3 = rhs.v3;

        return *this;
    }

};

#endif
