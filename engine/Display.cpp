#include "Display.h"

Display::Display(int width, int height) {
    
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

    initShaders();

    shaderProgram = new Shader("./shaders/vertex.glsl", "./shaders/fragment.glsl");

    calculateDisplaySettings();
}

Display::~Display() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram->ID);
    delete shaderProgram;
    glfwTerminate();
}

void Display::initShaders() {

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    vertexPixel00Pos = glGetUniformLocation(shaderProgram->ID, "pixel00Pos");
    vertexPixelDeltaU = glGetUniformLocation(shaderProgram->ID, "pixelDeltaU");
    vertexPixelDeltaV = glGetUniformLocation(shaderProgram->ID, "pixelDeltaV");

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

bool Display::renderLoop() {
    if (!glfwWindowShouldClose(window)) {

        glClearColor(0.2, 1.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        shaderProgram -> use();

        glUniform3f(vertexPixel00Pos, 
                    pixel00Loc.x(), pixel00Loc.y(), pixel00Loc.z());
        glUniform3f(vertexPixelDeltaU,
                    pixelDeltaU.x(), pixelDeltaU.y(), pixelDeltaU.z());
        glUniform3f(vertexPixelDeltaV,
                    pixelDeltaV.x(), pixelDeltaV.y(), pixelDeltaV.z());

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();    

        return true;

    }

    return false;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    // TODO: maybe get this to calculate display settings?
}
