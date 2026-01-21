#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <glm/glm.hpp>
#include "Surface.h"

struct Triangle {
    alignas(16) glm::vec3 position;
    alignas(16) glm::vec3 v1, v2, v3;

    Surface surface;
};

#endif // TRIANGLE_H
