#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Renderer {
    private:
        int windowWidth;
        int windowHeight;
    public:
        Renderer();
        void initGLFW();
        void initGLAD();
        GLFWwindow* createWindow(int width, int height);
};