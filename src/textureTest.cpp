#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <constants.h>
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
    glRenderer.setWindowSize(width, height);
    glRenderer.initGLFW();
    auto window = glRenderer.createWindow();
    glRenderer.initGLAD();
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //Create and bind a framebuffer objectshader
    unsigned int fbo[2], tex[2];
    glGenFramebuffers(2, fbo);
    glGenTextures(2, tex);
    for (int i = 0; i < 2; ++i) {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo[i]);

        glBindTexture(GL_TEXTURE_2D, tex[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex[i], 0);

        //Optionally check completeness
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
            std::cout << "Framebuffer " << i << " is complete!\n";
    }
    
    float quad[16] = {
        -1.0f, -1.0f, 0.0f, 0.0f,
        -1.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f, 0.0f
    };

    float smallSquare[8] = {
        -0.01f, -0.01f,
        -0.01f, 0.01f,
        0.01f, 0.01f,
        0.01f, -0.01f
    };

    unsigned int indices[6] = {
        0, 1, 2,
        2, 3, 0
    };
    unsigned int vao[2]; //Vertex array object
    glGenVertexArrays(2, vao);
    
    //Create GPU buffers
    //Full-screen quad
    glBindVertexArray(vao[0]);
    VertexBuffer vbf(quad, sizeof(quad));
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    IndexBuffer ibf(indices, 6);
    //Small square
    glBindVertexArray(vao[1]);
    VertexBuffer vbs(smallSquare, sizeof(smallSquare));
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
    glEnableVertexAttribArray(0);
    IndexBuffer ibs(indices, 6);

    //Initialize the shaders
    ShaderProgramSource basicSource = parseShader("shaders/basic.shader");
    unsigned int basicShader = createShader(basicSource.VertexSource, basicSource.FragmentSource);

    //ShaderProgramSource textureSource = parseShader("shaders/texture.shader");
    ShaderProgramSource textureSource = parseShader("shaders/texture.shader");
    unsigned int textureShader = createShader(textureSource.VertexSource, textureSource.FragmentSource);
    //glUseProgram(textureShader);

    ShaderProgramSource copyTextureSource = parseShader("shaders/copyTexture.shader");
    unsigned int copyTextureShader = createShader(copyTextureSource.VertexSource, copyTextureSource.FragmentSource);

    //Uniforms for texture to sample from and normalized pixel size
    int readIndex = 0;
    int writeIndex = 1;
    float texelSize = 0.004f;

    int uniformTexelLocation = glGetUniformLocation(textureShader, "texelSize");
    int uniformTextureLocation = glGetUniformLocation(textureShader, "inputTexture");

    //Main loop
    while(!glfwWindowShouldClose(window)) {
        processInput(window);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo[writeIndex]);
        glClear(GL_COLOR_BUFFER_BIT);
        //Bind the texture
        glUseProgram(textureShader);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex[readIndex]);

        glUniform1i(uniformTextureLocation, 0);
        glUniform1f(uniformTexelLocation, texelSize);

        glBindVertexArray(vao[0]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glUseProgram(basicShader);
        glBindVertexArray(vao[1]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(copyTextureShader);

        glBindVertexArray(vao[0]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();

        std::swap(readIndex, writeIndex);
    }
}