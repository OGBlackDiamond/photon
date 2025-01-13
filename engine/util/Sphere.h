#ifndef SPHERE_H
#define SPHERE_H

#include "Vector.h"

struct Sphere {

    Vector3 position;
    Vector3 color;
    Vector3 emissionColor;
    float radius;
    float emissionStrength;
    float smoothness;

    Sphere() {
        position = Vector3();
        color = Vector3();
        emissionColor = Vector3();
        radius = -1;
        emissionStrength = -1;
        smoothness = -1;
    }

    Sphere(Vector3 position, Vector3 color, Vector3 emissionColor,
           float radius, float emissionStrength, float smoothness) {
        this->position = position;
        this->color = color;
        this->emissionColor = emissionColor;
        this->radius = radius;
        this->emissionStrength = emissionStrength;
        this->smoothness = smoothness;
    }

    Sphere& operator=(const Sphere& rhs) {
        position = rhs.position;
        color = rhs.color;
        emissionColor = rhs.emissionColor;
        radius = rhs.radius;
        emissionStrength = rhs.emissionStrength;
        smoothness = rhs.smoothness;

        return *this;
    }

};

#endif
