#include "Display.h"

Display::Display(int width, int height) {

    iteration = 0;

    spheres = new Sphere[maxSpheres];
    
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    window = glfwCreateWindow(width, height, "Photon", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    glViewport(0, 0, width, width);

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);  

    shaderProgram = new Shader("./shaders/vertex.glsl", "./shaders/fragment.glsl");

    initShaders();

    calculateDisplaySettings();

}

Display::~Display() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &sphereSSBO);
    glDeleteBuffers(1, &triangleSSBO);
    glDeleteProgram(shaderProgram->ID);
    delete shaderProgram;
    delete []spheres;
    glfwTerminate();
}

void Display::initShaders() {

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &sphereSSBO);
    glGenBuffers(1, &triangleSSBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBindVertexArray(0);


    vertexPixel00Pos = glGetUniformLocation(shaderProgram->ID, "pixel00Pos");
    vertexPixelDeltaU = glGetUniformLocation(shaderProgram->ID, "pixelDeltaU");
    vertexPixelDeltaV = glGetUniformLocation(shaderProgram->ID, "pixelDeltaV");

    iterationCount = glGetUniformLocation(shaderProgram->ID, "iterationCount");

    sphereCount = glGetUniformLocation(shaderProgram->ID, "numSpheres");
    triCount = glGetUniformLocation(shaderProgram->ID, "numTris");

}

void Display::initSSBO() {

    // sphere SSBO
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, sphereSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER,
                 numSpheres * sizeof(Sphere),
                 spheres,
                 GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, sphereSSBO);

    // tri SSBO
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, triangleSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER,
               mesh.numTris * sizeof(Triangle),
               mesh.triArray,
               GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, triangleSSBO);
    
}

void Display::calculateDisplaySettings() {

    viewPortWidth = viewPortHeight  * (float(width) / height);
    viewPortU = glm::vec3(viewPortWidth, 0, 0);
    viewPortV = glm::vec3(0, -viewPortHeight, 0);

    pixelDeltaU = viewPortU / float(width);
    pixelDeltaV = viewPortV / float(height);

    viewPortUpperLeft = glm::vec3(0, 0, depth) - viewPortU/2.0f - viewPortV/2.0f;

    pixel00Loc = viewPortUpperLeft + 0.5f * (pixelDeltaU + pixelDeltaV);

}

bool Display::renderLoop() {

    //iteration++;
    /*
    spheres[0].position = Vector3(
        cos(iteration * 0.055) + 0.0,
        sin(iteration * 0.055) + 0.0,
        sin(iteration * 0.055) + 5.5
    );
    spheres[1].position = Vector3(
        sin(iteration * 0.055) + 0.0,
        cos(iteration * 0.055) + 0.0,
        sin(iteration * 0.055) + 5.5
    );
    spheres[2].position = Vector3(
        sin(iteration * 0.055) + 0.0,
        sin(iteration * 0.055) + 0.,
        cos(iteration * 0.055) + 5.5
    );
    */
    spheres[0].position = glm::vec3(
        -cos(iteration * .01) * 5,
        -sin(iteration * .01) * 5,
        -cos(iteration * .01) * 5 + 5
    );

    /*


    spheres[2].position = Vector3(
        cos(iteration * 0.0175),
        spheres[2].position.y(),
        spheres[2].position.z()
    );
    */

    if (!glfwWindowShouldClose(window)) {

        glClearColor(0.2, 1.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        shaderProgram -> use();

        glUniform3f(vertexPixel00Pos, 
                    pixel00Loc.x, pixel00Loc.y, pixel00Loc.z);
        glUniform3f(vertexPixelDeltaU,
                    pixelDeltaU.x, pixelDeltaU.y, pixelDeltaU.z);
        glUniform3f(vertexPixelDeltaV,
                    pixelDeltaV.x, pixelDeltaV.y, pixelDeltaV.z);

        glUniform1i(iterationCount, iteration);

        glUniform1i(sphereCount, numSpheres);

        glUniform1i(triCount, mesh.numTris);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        //glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();    
        

        return true;

    }

    return false;
}

void Display::setMesh(Mesh mesh) {
    this->mesh = mesh;
}

void Display::addSphere(Sphere sphere) {
    if (numSpheres == maxSpheres) {
        maxSpheres *= 2;
        Sphere* tmp = new Sphere[maxSpheres];
        for (int i = 0; i < numSpheres; i++) {
            tmp[i] = spheres[i];
        }
        delete[] spheres;
        spheres = tmp;
    }
    spheres[numSpheres] = sphere;
    numSpheres++;

}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    // TODO: maybe get this to calculate display settings?
}
