#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <cmath>

#include <indexBuffer.h>
#include <vertexBuffer.h>
#include <shaderLoader.h>
#include <renderer.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main() {
    //Initialize glfw window
    Renderer glRenderer = Renderer();
    glRenderer.initGLFW();
    auto window = glRenderer.createWindow(800, 600);
    glRenderer.initGLAD();
    //Set the method for changing window size
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    //Create the positions of the vertices
    float positions[8] = {
        -1.0f, -1.0f,
        -1.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, -1.0f
    };
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };
    //Create the vertex array object
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    //Create the buffer inside the GPU
    VertexBuffer vb1(positions, sizeof(positions));

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
    glEnableVertexAttribArray(0);
    IndexBuffer ib1(indices, 6);

    ShaderProgramSource shaderSource = parseShader("shaders/basic.shader");

    unsigned int shader = createShader(shaderSource.VertexSource, shaderSource.FragmentSource);
    glUseProgram(shader);

    int location = glGetUniformLocation(shader, "vertexColorUniform");
    
    glm::vec3 colors[5];

    while(!glfwWindowShouldClose(window))
    {
        processInput(window);

        float time = glfwGetTime();
        for (int i = 0; i < 5; ++i) {
            colors[i] = glm::vec3(
                0.5f + 0.5f * sin(time + i),
                0.5f + 0.5f * sin(time + i + 1.05),
                0.5f + 0.5f * sin(time + i + 2.1)
            );
        }

        glClear(GL_COLOR_BUFFER_BIT);

        glUniform3fv(location, 5, &colors[0][0]);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(window);
        glfwPollEvents();    
    }
    glfwTerminate();
    return 0;
    //Create the index array for drawing two triangles
    /*unsigned int indices[] = {
        0, 1, 2,
        0, 2, 3
    };*/
}