#ifndef SPHERE_H
#define SPHERE_H

#include <glm/glm.hpp>

#include "Surface.h"


struct Sphere {
    glm::vec3 position;
    float radius;

    Surface surface;
};

#endif
