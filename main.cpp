#include "engine/Display.h"
#include "engine/util/Mesh.h"

Sphere sphere1 = {
    glm::vec3(0.25, 0.25, 3.25),
    0.5,
    {
        glm::vec3(1, 0.2, 0.2),
        glm::vec3(0, 0, 0),
        0,
        0
    }
};

Sphere sphere2 = {
    glm::vec3(0.5, 0.5, 0.5),
    0.35,
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

Sphere sphere4 = {
    glm::vec3(-0.5, 0.45, 2.7),
    0.1,
    {
        glm::vec3(0.2, 1, 0.2),
        glm::vec3(0, 0, 0),
        0,
        0
    }
};

Sphere sphere5 = {
    glm::vec3(-0.5, 0.15, 2.7),
    0.1,
    {
        glm::vec3(1, 1, 1),
        glm::vec3(0, 0, 0),
        0,
        0
    }
};

Sphere ground = {
    glm::vec3(0, -100.5, 1.0),
    100,
    {
        glm::vec3(1, 0.4, 0.4),
        glm::vec3(0, 0, 0),
        0,
        0.999
    }
};

Sphere wall = {
    glm::vec3(101.5, 0, 1.0),
    100,
    {
        glm::vec3(0.4, 1, 0.4),
        glm::vec3(0, 0, 0),
        0,
        0.999
    }
};

Sphere wall2 = {
    glm::vec3(-101.5, 0, 1.0),
    100,
    {
        glm::vec3(0.4, 0.4, 1),
        glm::vec3(0, 0, 0),
        0,
        0.999
    }
};

Sphere wall3 = {
    glm::vec3(0, 0, 105.5),
    100,
    {
        glm::vec3(1, 1, 1),
        glm::vec3(0, 0, 0),
        0,
        0.999
    }
};

Sphere wall4 = {
    glm::vec3(0, 0, -101.5),
    100,
    {
        glm::vec3(1, 1, 1),
        glm::vec3(0, 0, 0),
        0,
        0.999
    }
};



Sphere ceiling = {
    glm::vec3(0, 102, 1.0),
    100,
    {
        glm::vec3(0.4, 0.4, 1),
        glm::vec3(1, 1, 1),
        0.1,
        0
    }
};




Mesh mesh("./meshes/monkey.obj",
            glm::vec3(0, 0.5, 4),
            {
                glm::vec3(1, 1, 1),
                glm::vec3(0, 0, 0),
                0,
                0
            }
        );

int main(int argc, char** argv) {
    Display display(2000, 2000);
    display.setMesh(&mesh);
    //display.addSphere(sphere1);
    display.addSphere(ground);
    display.addSphere(wall);
    display.addSphere(wall2);
    display.addSphere(wall3);
    display.addSphere(wall4);
    display.addSphere(ceiling);
    //display.addSphere(sphere2);
    /*
    display.addSphere(sphere3);
    display.addSphere(sphere4);
    display.addSphere(sphere5);
    */

    display.initSSBO();
    //display.addSphere(sphere1);
    while (display.renderLoop()) {}
    return 0;
}
