#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Renderer {
    private:
        int windowWidth;
        int windowHeight;
    public:
        Renderer();
        void setWindowSize(int width, int height);
        void initGLFW(int majorVersion, int minorVersion);
        void initGLAD();
        GLFWwindow* createWindow(bool fullscreen);
};