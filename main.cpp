#include "engine/Display.h"
#include "engine/util/Mesh.h"

Sphere sphere1 = {
    glm::vec3(0, 0.5, 3.25),
    0.5,
    {
        glm::vec3(1, 0.2, 0.2),
        glm::vec3(0, 0, 0),
        0,
        0
    }
};

Sphere sphere2 = {
    glm::vec3(-5, 10, -7),
    3.5,
    {
        glm::vec3(0, 0, 0),
        glm::vec3(1, 1, 1),
        1,
        0
    }
};

Sphere sphere3 = {
    glm::vec3(-0.5, 0.75, 2.7),
    0.1,
    {
        glm::vec3(0.2, 0.2, 1),
        glm::vec3(0, 0, 0),
        0,
        0
    }
};

Sphere ground = {
    glm::vec3(0, -100.5, 1.0),
    100,
    {
        glm::vec3(0.2, 1, 0.2),
        glm::vec3(0, 0, 0),
        0,
        0
    }
};

Mesh mesh("./meshes/monkey.obj",
            glm::vec3(0, 1, 3),
            {
                glm::vec3(0.8, 0.2, 0.8),
                glm::vec3(1, 1, 1),
                0,
                0
            }
        );

int main(int argc, char** argv) {
    Display display(500, 500);
    //display.setMesh(mesh);
    display.addSphere(sphere1);
    display.addSphere(ground);
    display.addSphere(sphere2);
    display.addSphere(sphere3);

    display.initSSBO();
    //display.addSphere(sphere1);
    while (display.renderLoop()) {}
    return 0;
}
