#include "engine/Display.h"

int main(int argc, char** argv) {
    Display display(500, 500);
    while (display.renderLoop()) {}
    return 0;
}

