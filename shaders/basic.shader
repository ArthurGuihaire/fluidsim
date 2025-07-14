#shader vertex
#version 330 core
uniform vec2 offset;
layout(location = 0) in vec2 position;
void main() {
    gl_Position = vec4(position + offset, 0.0, 1.0);
};

#shader fragment
#version 330 core
uniform vec3 inputColor;
out vec4 fragColor;

void main() {
    fragColor = vec4(inputColor, 0.0f);
};