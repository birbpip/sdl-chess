#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

// Window dimensions
const GLint WIDTH = 800, HEIGHT = 600;

// Cube vertices and indices
GLfloat vertices[] = {
    // Positions         
    -0.5f, -0.5f, -0.5f,  
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f
};

// Indices for drawing the cube
GLuint indices[] = {
    0, 1, 2, 2, 3, 0,  // Front
    4, 5, 6, 6, 7, 4,  // Back
    4, 5, 1, 1, 0, 4,  // Bottom
    3, 2, 6, 6, 7, 3,  // Top
    4, 0, 3, 3, 7, 4,  // Left
    1, 5, 6, 6, 2, 1   // Right
};

// Shaders (Vertex and Fragment)
const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 position;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main() {
    gl_Position = projection * view * model * vec4(position, 1.0f);
}
)";

const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
void main() {
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);  // Orange color
}
)";

// Function to compile and link shaders
GLuint compileShader(const char* shaderSource, GLenum shaderType) {
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSource, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cout << "Shader Compilation Failed:\n" << infoLog << std::endl;
    }
    return shader;
}

// Initialize SDL, OpenGL, and set up the cube
void initOpenGL(SDL_Window* &window, SDL_GLContext &context, GLuint &VAO, GLuint &VBO, GLuint &EBO, GLuint &shaderProgram) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        exit(1);
    }

    window = SDL_CreateWindow("OpenGL Cube", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_OPENGL);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        exit(1);
    }

    context = SDL_GL_CreateContext(window);
    if (!context) {
        std::cerr << "OpenGL context could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        exit(1);
    }

    if (glewInit() != GLEW_OK) {
        std::cerr << "GLEW could not be initialized!" << std::endl;
        exit(1);
    }

    glEnable(GL_DEPTH_TEST);  // Enable depth testing for 3D rendering

    // Vertex Shader
    GLuint vertexShader = compileShader(vertexShaderSource, GL_VERTEX_SHADER);

    // Fragment Shader
    GLuint fragmentShader = compileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);

    // Shader Program
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    // Cube Setup
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);  // Use the passed reference VBO
    glGenBuffers(1, &EBO);  // Use the passed reference EBO

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


// Rendering function (including rotation)
void render(SDL_Window* window, GLuint shaderProgram, GLuint VAO, GLuint modelLoc, GLuint viewLoc, GLuint projectionLoc, float angle) {
    // Clear the screen and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Use the shader program
    glUseProgram(shaderProgram);

    // Set the model matrix for rotation
    glm::mat4 model = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.5f, 1.0f, 0.0f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    // Set the view matrix for camera position
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    // Set the projection matrix for perspective view
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // Render the cube
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);  // 36 indices for the cube
    glBindVertexArray(0);

    // Swap the buffer to display the rendered frame
    SDL_GL_SwapWindow(window);
}

int main() {
    SDL_Window* window;
    SDL_GLContext context;
    GLuint VAO, VBO, EBO, shaderProgram;

    // Initialize OpenGL and create the cube
    initOpenGL(window, context, VAO, VBO, EBO, shaderProgram);

    // Get uniform locations
    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
    GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLuint projectionLoc = glGetUniformLocation(shaderProgram, "projection");

    // Main loop
    bool running = true;
    float angle = 0.0f;
    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = false;
            }
        }

        // Increment rotation angle
        angle += 0.01f;
        if (angle >= 360.0f) angle = 0.0f;

        // Render the scene
        render(window, shaderProgram, VAO, modelLoc, viewLoc, projectionLoc, angle);
    }

    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
