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
    glDeleteProgram(shaderProgram->ID);
    delete shaderProgram;
    delete []spheres;
    glfwTerminate();
}

void Display::initShaders() {

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);

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

    sphereVectors = glGetUniformLocation(shaderProgram->ID, "sphereVectors");
    sphereFloats = glGetUniformLocation(shaderProgram->ID, "sphereFloats");

    triPoints = glGetUniformLocation(shaderProgram->ID, "trianglePoints");
    triVectors = glGetUniformLocation(shaderProgram->ID, "triangleVectors");
    triFloats = glGetUniformLocation(shaderProgram->ID, "triangleFloats");


    sphereCount = glGetUniformLocation(shaderProgram->ID, "numSpheres");
    triCount = glGetUniformLocation(shaderProgram->ID, "numTris");

}

void Display::calculateDisplaySettings() {

    viewPortWidth = viewPortHeight  * (float(width) / height);
    viewPortU = Vector3(viewPortWidth, 0, 0);
    viewPortV = Vector3(0, -viewPortHeight, 0);

    pixelDeltaU = viewPortU / double(width);
    pixelDeltaV = viewPortV / double(height);

    viewPortUpperLeft = Vector3(0, 0, depth) - viewPortU/2 - viewPortV/2;

    pixel00Loc = viewPortUpperLeft + 0.5 * (pixelDeltaU + pixelDeltaV);

}

void Display::initializeSphereInformation() {
    //delete [] sphereMatrixFloats; delete [] sphereVectorFloats;
    sphereMatrixFloats = new float[numSpheres * 9];
    sphereVectorFloats = new float[numSpheres * 3];
    for (int i = 0; i < numSpheres; i++) {
        sphereMatrixFloats[i * 9] = spheres[i].position.x();
        sphereMatrixFloats[i * 9 + 1] = spheres[i].position.y();
        sphereMatrixFloats[i * 9 + 2] = spheres[i].position.z();
        sphereMatrixFloats[i * 9 + 3] = spheres[i].color.x();
        sphereMatrixFloats[i * 9 + 4] = spheres[i].color.y();
        sphereMatrixFloats[i * 9 + 5] = spheres[i].color.z();
        sphereMatrixFloats[i * 9 + 6] = spheres[i].emissionColor.x();
        sphereMatrixFloats[i * 9 + 7] = spheres[i].emissionColor.y();
        sphereMatrixFloats[i * 9 + 8] = spheres[i].emissionColor.z();

        sphereVectorFloats[i * 3] = spheres[i].radius;
        sphereVectorFloats[i * 3 + 1] = spheres[i].emissionStrength;
        sphereVectorFloats[i * 3 + 2] = spheres[i].smoothness;
        
    } 
}

void Display::initializeTriangleInformation() {
    triangleMatrixPoints = new float[mesh.numTris * 9];
    triangleMatrixVectors = new float[mesh.numTris * 9];
    triangleVectorFloats = new float[mesh.numTris * 2];
    for (int i = 0; i < mesh.numTris; i++) {
        triangleMatrixPoints[i * 9] = mesh.triArray[i].v1.x();
        triangleMatrixPoints[i * 9 + 1] = mesh.triArray[i].v1.y();
        triangleMatrixPoints[i * 9 + 2] = mesh.triArray[i].v1.z();
        triangleMatrixPoints[i * 9 + 3] = mesh.triArray[i].v2.x();
        triangleMatrixPoints[i * 9 + 4] = mesh.triArray[i].v2.y();
        triangleMatrixPoints[i * 9 + 5] = mesh.triArray[i].v2.z();
        triangleMatrixPoints[i * 9 + 6] = mesh.triArray[i].v3.x();
        triangleMatrixPoints[i * 9 + 7] = mesh.triArray[i].v3.y();
        triangleMatrixPoints[i * 9 + 8] = mesh.triArray[i].v3.z();
        

        triangleMatrixVectors[i * 9] = mesh.position.x();
        triangleMatrixVectors[i * 9 + 1] = mesh.position.y();
        triangleMatrixVectors[i * 9 + 2] = mesh.position.z();
        triangleMatrixVectors[i * 9 + 3] = mesh.color.x();
        triangleMatrixVectors[i * 9 + 4] = mesh.color.y();
        triangleMatrixVectors[i * 9 + 5] = mesh.color.z();
        triangleMatrixVectors[i * 9 + 6] = mesh.emissionColor.x();
        triangleMatrixVectors[i * 9 + 7] = mesh.emissionColor.y();
        triangleMatrixVectors[i * 9 + 8] = mesh.emissionColor.z();

        triangleVectorFloats[i * 2] = mesh.emissionStrength;
        triangleVectorFloats[i * 2 + 1] = mesh.smoothness;

    }
}

bool Display::renderLoop() {

    iteration++;
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
        sin(iteration * 0.055) + 0.0,
        cos(iteration * 0.055) + 5.5
    );
    spheres[3].position = Vector3(
        cos(iteration * .01) * 5,
        sin(iteration * .01) * 5,
        cos(iteration * .01) * 5 + 5
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

        initializeSphereInformation();
        initializeTriangleInformation();

        shaderProgram -> use();

        glUniform3f(vertexPixel00Pos, 
                    pixel00Loc.x(), pixel00Loc.y(), pixel00Loc.z());
        glUniform3f(vertexPixelDeltaU,
                    pixelDeltaU.x(), pixelDeltaU.y(), pixelDeltaU.z());
        glUniform3f(vertexPixelDeltaV,
                    pixelDeltaV.x(), pixelDeltaV.y(), pixelDeltaV.z());

        glUniform1i(iterationCount, iteration);

        glUniform1i(sphereCount, numSpheres);

        glUniform1i(triCount, mesh.numTris);


        glUniformMatrix3fv(
            sphereVectors,
            numSpheres,
            GL_FALSE,
            sphereMatrixFloats
        );

        glUniform3fv(
            sphereFloats,
            numSpheres,
            sphereVectorFloats
        );

        glUniformMatrix3fv(
            triPoints,
            mesh.numTris,
            GL_FALSE,
            triangleMatrixPoints 
        );

        glUniformMatrix3fv(
            triVectors,
            mesh.numTris,
            GL_FALSE,
            triangleMatrixVectors
        );

        glUniform2fv(
            triFloats,
            mesh.numTris,
            triangleVectorFloats
        );

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
