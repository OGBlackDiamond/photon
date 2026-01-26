#include "Display.h"

Display::Display(int width, int height) {

    this->width = width;
    this->height = height;

    iteration = 0;

    spheres = new Sphere[maxSpheres];
    
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    glfwSwapInterval(1); // or 0 for testing

    
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

    glViewport(0, 0, width, height);

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
    glDeleteFramebuffers(2, fbo);
    glDeleteTextures(2, tex);
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

    // Ping-pong FBO and texture setup
    glGenFramebuffers(2, fbo);
    glGenTextures(2, tex);
    for (int i = 0; i < 2; i++) {
        glBindTexture(GL_TEXTURE_2D, tex[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_2D, 0);  // Unbind texture

        glBindFramebuffer(GL_FRAMEBUFFER, fbo[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex[i], 0);
        //GLenum drawBuffers[] = {GL_COLOR_ATTACHMENT0};
        //glDrawBuffers(1, drawBuffers);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cout << "FBO " << i << " not complete!" << std::endl;
        }
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);  // Unbind

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

    frameCountLoc = glGetUniformLocation(shaderProgram->ID, "frameCount");
    isAccumulatingLoc = glGetUniformLocation(shaderProgram->ID, "isAccumulating");
    prevTexLoc = glGetUniformLocation(shaderProgram->ID, "prevTex");
    accumTexLoc = glGetUniformLocation(shaderProgram->ID, "accumTex");
    screenWidthLoc = glGetUniformLocation(shaderProgram->ID, "screenWidth");
    screenHeightLoc = glGetUniformLocation(shaderProgram->ID, "screenHeight");

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
    viewPortV = glm::vec3(0, viewPortHeight, 0);

    pixelDeltaU = viewPortU / float(width);
    pixelDeltaV = viewPortV / float(height);

    viewPortUpperLeft = glm::vec3(0, 0, depth) - viewPortU/2.0f - viewPortV/2.0f;

    pixel00Loc = viewPortUpperLeft + 0.5f * (pixelDeltaU + pixelDeltaV);

}

bool Display::renderLoop() {

    if (!glfwWindowShouldClose(window)) {

        iteration++;  // Increment each frame
        int frameCount = iteration;
        int readIdx = (iteration + 1) % 2;
        int writeIdx = iteration % 2;

        // Pass 1: Accumulate to ping-pong FBO
        glBindFramebuffer(GL_FRAMEBUFFER, fbo[writeIdx]);
        glClear(GL_COLOR_BUFFER_BIT);
        shaderProgram->use();
        glUniform1i(isAccumulatingLoc, 1);
        glUniform1i(frameCountLoc, frameCount);
        glUniform1i(screenWidthLoc, width);
        glUniform1i(screenHeightLoc, height);
        glUniform1i(prevTexLoc, 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex[readIdx]);
        glUniform3f(vertexPixel00Pos, pixel00Loc.x, pixel00Loc.y, pixel00Loc.z);
        glUniform3f(vertexPixelDeltaU, pixelDeltaU.x, pixelDeltaU.y, pixelDeltaU.z);
        glUniform3f(vertexPixelDeltaV, pixelDeltaV.x, pixelDeltaV.y, pixelDeltaV.z);
        glUniform1i(sphereCount, numSpheres);
        glUniform1i(triCount, mesh.numTris);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        //TODO: update to GLAD 4.5
        /*
        glMemoryBarrier(GL_FRAMEBUFFER_BARRIER_BIT |
                GL_TEXTURE_FETCH_BARRIER_BIT |
                GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
        */

        // Pass 2: Display to screen
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(0.2, 0.0, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        shaderProgram->use();
        glUniform1i(isAccumulatingLoc, 0);
        glUniform1i(accumTexLoc, 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex[writeIdx]);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();

        glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
        glFlush(); // NOT glFinish()

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
