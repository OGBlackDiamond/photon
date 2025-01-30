#include "engine/Display.h"
#include "engine/util/Mesh.h"

Sphere sphere1(
    Vector3(0, 0.5, 3.25),
    Vector3(1, 0.2, 0.2),
    Vector3(1, 1, 1),
    0.5,
    1,
    0.2 
);

Sphere sphere2(
    Vector3(1.75, 0, 3),
    Vector3(0, 0, 0),
    Vector3(1, 1, 1),
    3.5,
    2,
    0
);

Sphere sphere3(
    Vector3(0, 0.5, 4),
    Vector3(0.2, 1, 1),
    Vector3(1, 1, 1),
    1,
    0,
    0
);

Sphere ground(
    Vector3(0, -100.5, 1.0),
    Vector3(1, 1, 1),
    Vector3(1, 1, 1),
    100,
    0,
    0
);

Mesh mesh("./meshes/box2.obj",
          Vector3(1, 1, 9),
          Vector3(0.8, 0.2, 0.8),
          Vector3(1, 1, 1),
          1,
          0);


int main(int argc, char** argv) {
    Display display(500, 500);
    display.setMesh(mesh);
    /*
    display.addSphere(sphere1);
    display.addSphere(sphere1);
    display.addSphere(sphere1);
    */
    display.addSphere(sphere2);
    //display.addSphere(sphere1);
    //display.addSphere(ground);
    while (display.renderLoop()) {}
    return 0;
}

