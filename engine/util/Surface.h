#ifndef SURFACE_H
#define SURFACE_H

#include <glm/glm.hpp>

struct Surface {
    alignas(16) glm::vec3 surfaceColor;
    alignas(16) glm::vec3 emissionColor;
    float emissionStrength;
    float smoothness;
};

#endif // SURFACE_H
