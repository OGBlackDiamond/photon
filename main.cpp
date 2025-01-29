#include "engine/Display.h"
#include "engine/util/Mesh.h"

Sphere sphere1(
    Vector3(-0.75, 0.5, 3),
    Vector3(1, 0.2, 0.2),
    Vector3(1, 1, 1),
    0.5,
    0,
    0
);

Sphere sphere2(
    Vector3(1.75, 0, 3),
    Vector3(0, 0, 0),
    Vector3(1, 1, 1),
    4.5,
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

Mesh mesh("./meshes/decimonkey.obj",
          Vector3(0, 0, 3),
          Vector3(160.0/255, 90.0/255, 62.0/255),
          Vector3(1, 1, 1),
          0,
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
    //display.addSphere(ground);
    while (display.renderLoop()) {}
    return 0;
}

