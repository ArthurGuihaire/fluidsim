#shader vertex
#version 330 core
layout(location = 0) in vec2 position;
void main() {
    gl_Position = vec4(position, 0.0, 1.0);
};

#shader fragment
#version 330 core
uniform vec3 inputColor;
out vec3 fragColor;

void main() {
    fragColor = inputColor;
};