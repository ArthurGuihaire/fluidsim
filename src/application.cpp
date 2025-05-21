#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

#include <indexBuffer.h>
#include <vertexBuffer.h>

struct ShaderProgramSource {
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath) {
    std::ifstream stream(filepath);

    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };
    ShaderType type = ShaderType::NONE;

    std::string line;
    std::stringstream ss[2];
    std::cout << "Trying to open file: " << filepath << '\n';
    if (!stream.is_open()) {
        std::cerr << "Failed to open shader file: " << filepath << '\n';
        return {};
    }
    
    while (getline(stream, line)) {
        std::cout << line;
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if(line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else {
            ss[(int) type] << line << "\n";
        }
    }
    return { ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "failed to compile " << 
            (type == GL_VERTEX_SHADER ? "vertex" : "fragment") <<
            "shader" << message << std::endl;

        glDeleteShader(id);
        return 0;
    }

    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main() {
    //Initialize glfw window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    //Create an OpenGL contex for the window
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glViewport(0, 0, 800, 600);
    //Set the method for changing window size
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    //Create the positions of the vertices
    float positions[8] = {
        -0.5f, -0.5f,
        -0.5f, 0.5f,
        0.5f, 0.5f,
        0.5f, -0.5f
    };
    //Create the index array for drawing two triangles
    unsigned int indices1[] = {
        0, 1, 2
    };
    unsigned int indices2[] = {
        0, 2, 3
    };
    //Create the vertex array object
    GLuint vao[2];
    glGenVertexArrays(2, vao);
    glBindVertexArray(vao[0]);
    //Create the buffer inside the GPU

    VertexBuffer vb1(positions, 4*2*sizeof(float));

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
    glEnableVertexAttribArray(0);
    IndexBuffer ib1(indices1, 3);

    glBindVertexArray(vao[1]);

    VertexBuffer vb2(positions, 4*2*sizeof(float));

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
    glEnableVertexAttribArray(0);
    IndexBuffer ib2(indices2, 3);

    ShaderProgramSource shaderSource = ParseShader("shaders/basic.shader");

    unsigned int shader = CreateShader(shaderSource.VertexSource, shaderSource.FragmentSource);
    glUseProgram(shader);

    int location = glGetUniformLocation(shader, "u_Color");
    //ASSERT(location!=-1);
    float red = 0.0f;
    float increment = 0.01f;

    while(!glfwWindowShouldClose(window))
    {
        processInput(window);

        red += increment;
        if (red >= 1.0f || red <= 0.0f) {
            increment = -increment;
        }

        glClear(GL_COLOR_BUFFER_BIT);

        glUniform4f(location, red, 0.3f, 0.8f, 1.0f);

        glBindVertexArray(vao[0]);

        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

        glBindVertexArray(vao[1]);

        glUniform4f(location, 0.0f, red, 0.8f, 1.0f);

        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(window);
        glfwPollEvents();    
    }
    glfwTerminate();
    return 0;
}