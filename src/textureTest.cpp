#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/common.hpp>
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

    //Define normalized pixel (texel) sizes
    float texelSizeX = 1.0f/width;
    float texelSizeY = 1.0f/height;

    //Create and bind a framebuffer objectshader
    unsigned int fbo[2], tex[2];
    glGenFramebuffers(2, fbo);
    glGenTextures(2, tex);
    for (int i = 0; i < 2; ++i) {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo[i]);

        glBindTexture(GL_TEXTURE_2D, tex[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex[i], 0);

        //Check completeness
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
            std::cout << "Framebuffer " << i << " is complete!" << std::endl;
    }

    glDisable(GL_BLEND); //Blending causes problems for some reason

    float normBorderX = borderWidth * texelSizeX;
    float normBorderY = borderWidth * texelSizeY;
    normBorderX = 0.0f;
    normBorderY = 0.0f;
    
    float quad[16] = {
        -1.0f + normBorderX, -1.0f + normBorderY, 0.0f + normBorderX / 2, 0.0f + normBorderY / 2,
        -1.0f + normBorderX, 1.0f - normBorderY, 0.0f + normBorderX / 2, 1.0f - normBorderY / 2,
        1.0f - normBorderX, 1.0f - normBorderY, 1.0f - normBorderX / 2, 1.0f - normBorderY / 2,
        1.0f - normBorderX, -1.0f + normBorderY, 1.0f - normBorderX / 2, 0.0f + normBorderY / 2
    };

    //Muliply by normalized pixel sizes so its actually a square
    float smallSquare[8] = {
        -50 * texelSizeX, -50 * texelSizeY,
        -50 * texelSizeX, 50 * texelSizeY,
        50 * texelSizeX, 50 * texelSizeY,
        50 * texelSizeX, -50 * texelSizeY
    };

    float blackBorder[16] = {
        -1.0f, -1.0f, 
        -1.0f, 1.0f, 
        1.0f, 1.0f, 
        1.0f, -1.0f, 
        -1.0f + normBorderX, -1.0f + normBorderY, 
        -1.0f + normBorderX, 1.0f - normBorderY, 
        1.0f - normBorderX, 1.0f - normBorderY, 
        1.0f - normBorderX, -1.0f + normBorderY
    };

    unsigned int indicesQuad[6] = {
        0, 1, 2,
        2, 3, 0
    };

    unsigned int indicesBorder[24] = {
        0, 1, 5, 
        0, 4, 5, 
        1, 2, 6, 
        1, 5, 6, 
        2, 7, 3, 
        2, 6, 7, 
        3, 4, 0, 
        3, 7, 4
    };

    unsigned int vao[3]; //Vertex array objects for fullscreen quad and small square
    glGenVertexArrays(3, vao);
    
    //Create GPU buffers
    //Full-screen quad
    glBindVertexArray(vao[0]);
    VertexBuffer vbf(quad, sizeof(quad));
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    IndexBuffer ibf(indicesQuad, 6);
    //Small square
    glBindVertexArray(vao[1]);
    VertexBuffer vbs(smallSquare, sizeof(smallSquare));
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
    glEnableVertexAttribArray(0);
    IndexBuffer ibs(indicesQuad, 6);
    glBindVertexArray(vao[2]);
    VertexBuffer vbb(blackBorder, sizeof(blackBorder));
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
    glEnableVertexAttribArray(0);
    IndexBuffer ibb(indicesBorder, 24);
    //Initialize the shaders
    ShaderProgramSource basicSource = parseShader("shaders/basic.shader");
    unsigned int basicShader = createShader(basicSource.VertexSource, basicSource.FragmentSource);

    //ShaderProgramSource textureSource = parseShader("shaders/texture.shader");
    ShaderProgramSource textureSource = parseShader("shaders/advection.shader");
    unsigned int advectionShader = createShader(textureSource.VertexSource, textureSource.FragmentSource);
    //glUseProgram(advectionShader);

    ShaderProgramSource copyTextureSource = parseShader("shaders/copyTexture.shader");
    unsigned int copyTextureShader = createShader(copyTextureSource.VertexSource, copyTextureSource.FragmentSource);

    ShaderProgramSource blurSource = parseShader("shaders/blur.shader");
    unsigned int blurShader = createShader(blurSource.VertexSource, blurSource.FragmentSource);

    ShaderProgramSource divergenceSource = parseShader("shaders/divergence.shader");
    unsigned int divergenceShader = createShader(divergenceSource.VertexSource, divergenceSource.FragmentSource);

    ShaderProgramSource pressureSource = parseShader("shaders/pressure.shader");
    unsigned int pressureShader = createShader(pressureSource.VertexSource, pressureSource.FragmentSource);

    //Uniforms for texture to sample from and normalized pixel size
    int readIndex = 0;
    int writeIndex = 1;
    bool renderStartingSquare = true;

    int uniformTexelLocationAdvection = glGetUniformLocation(advectionShader, "texelSize");
    int uniformTextureLocation = glGetUniformLocation(advectionShader, "inputTexture");
    int uniformTexelLocationBlur = glGetUniformLocation(blurShader, "texelSize");
    int uniformTextureLocationBlur = glGetUniformLocation(blurShader, "inputTexture");
    int uniformColorLocationBasic = glGetUniformLocation(basicShader, "inputColor");
    int uniformOffsetLocationBasic = glGetUniformLocation(basicShader, "offset");
    int uniformTextureLocationDivergence = glGetUniformLocation(divergenceShader, "inputTexture");
    int uniformTexelLocationDivergence = glGetUniformLocation(divergenceShader, "texelSize");
    int uniformTextureLocationPressure = glGetUniformLocation(pressureShader, "inputTexture");
    int uniformTexelLocationPressure = glGetUniformLocation(pressureShader, "texelSize");
    int uniformTextureLocationCopy = glGetUniformLocation(copyTextureShader, "inputTexture");

    //Mouse position variables
    double mouseX, mouseY;
    double normMouseX, normMouseY;
    double mouseVelX = 0, mouseVelY = 0;


    //Main loop
    while(!glfwWindowShouldClose(window)) {
        processInput(window);
        glfwGetCursorPos(window, &mouseX, &mouseY);
        mouseVelX = 2 * mouseX / width - 1 - normMouseX;
        mouseVelY = -2 * mouseY / height + 1 - normMouseY;
        normMouseX = 2 * mouseX / width - 1;
        normMouseY = -2 * mouseY / height + 1;

        //std::cout << "X: " << mouseVelX << ", Y: " << mouseVelY << std::endl;

        //Bind texture to read from and framebuffer to write to the other texture
        glBindVertexArray(vao[0]);
        for (int i = 1; i < 8; i++){
            //Blur first so advection can self-spread momentum
            /*glUseProgram(blurShader);
            glBindFramebuffer(GL_FRAMEBUFFER, fbo[writeIndex]);
            glClear(GL_COLOR_BUFFER_BIT);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, tex[readIndex]);

            glUniform1i(uniformTextureLocationBlur, 0);
            glUniform2f(uniformTexelLocationBlur, texelSizeX * i, texelSizeY * i);

            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); //Single advection pass
            std::swap(readIndex, writeIndex);*/
            
            //Pressure calculation after blur
            glUseProgram(pressureShader);
            glBindFramebuffer(GL_FRAMEBUFFER, fbo[writeIndex]);
            glClear(GL_COLOR_BUFFER_BIT);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, tex[readIndex]);

            glUniform1i(uniformTextureLocationPressure, 0);
            glUniform2f(uniformTexelLocationPressure, texelSizeX * (i+1), texelSizeY * (i+1));

            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            std::swap(readIndex, writeIndex);

            //Zero out divergence from pressure
            glUseProgram(divergenceShader);
            glBindFramebuffer(GL_FRAMEBUFFER, fbo[writeIndex]);
            glClear(GL_COLOR_BUFFER_BIT);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, tex[readIndex]);

            glUniform1i(uniformTextureLocationDivergence, 0);
            glUniform2f(uniformTexelLocationDivergence, texelSizeX * i, texelSizeY * i);

            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            std::swap(readIndex, writeIndex);

            //Advection to simulate momentum
            glUseProgram(advectionShader);
            glBindFramebuffer(GL_FRAMEBUFFER, fbo[writeIndex]);
            glClear(GL_COLOR_BUFFER_BIT);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, tex[readIndex]);

            glUniform1i(uniformTextureLocation, 0);
            glUniform2f(uniformTexelLocationDivergence, texelSizeX * i, texelSizeY * i);

            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            std::swap(readIndex, writeIndex);
        }
        //std::swap(readIndex, writeIndex);

        glUseProgram(basicShader);
        if (renderStartingSquare)
        {
            glBindVertexArray(vao[0]); //Use basicShader to fill a texture with zero velocity and black color
            glUniform3f(uniformColorLocationBasic, 0.5, 0.5, 0.0); //0.5 is black for red and green channels
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            renderStartingSquare = false;
        }
        if (mouseVelX != 0 && mouseVelY != 0) {
            glBindVertexArray(vao[1]);
            glUniform3f(uniformColorLocationBasic, mouseVelX * 80 + 0.5, mouseVelY * 80 + 0.5, 0.0);
            glUniform2f(uniformOffsetLocationBasic, normMouseX, normMouseY);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(copyTextureShader);
        glUniform1i(uniformTextureLocationCopy, 0);

        glBindVertexArray(vao[0]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();

        //std::swap(readIndex, writeIndex);
    }
}