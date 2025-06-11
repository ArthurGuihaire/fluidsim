#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <indexBuffer.h>
#include <vertexBuffer.h>
#include <shaderLoader.h>
#include <initializer.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main() {
    Renderer glRenderer = Renderer();
    glRenderer.initGLFW();
    auto window = glRenderer.createWindow(800, 600);
    glRenderer.initGLAD();
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //Create and bind a framebuffer object
    unsigned int fbo[2], tex[2];
    glGenFramebuffers(2, fbo);
    glGenTextures(2, tex);
    for (int i = 0; i < 2; ++i) {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo[i]);

        glBindTexture(GL_TEXTURE_2D, tex[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex[i], 0);

        //Optionally check completeness
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
            std::cout << "Framebuffer " << i << " is complete!\n";
    }
    
    float quad[8] = {
        -1.0f, -1.0f,
        -1.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, -1.0f
    };

    unsigned int indices[6] = {
        0, 1, 2,
        2, 3, 0
    };
    unsigned int vao; //Vertex array object
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    //Create GPU buffers
    VertexBuffer vb(quad, sizeof(quad));
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
    glEnableVertexAttribArray(0);glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    IndexBuffer ib(indices, 6);

    //Initialize the shaders
    ShaderProgramSource shaderSource = parseShader("shaders/texture.shader");
    unsigned int shader = createShader(shaderSource.VertexSource, shaderSource.FragmentSource);
    glUseProgram(shader);

    int readIndex = 0;
    int writeIndex = 1;

    int uniformLocation = glGetUniformLocation(shader, "inputTexture");
    
    //Main loop
    while(!glfwWindowShouldClose(window)) {
        processInput(window);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo[writeIndex]);
        glClear(GL_COLOR_BUFFER_BIT);
        //Bind the texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex[readIndex]);

        glUniform1i(uniformLocation, 0);

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex[writeIndex]);
        glUniform1i(uniformLocation, 0);

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();    

        std::swap(readIndex, writeIndex);
    }
}