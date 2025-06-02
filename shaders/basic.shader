#shader vertex
#version 330 core
layout(location = 0) in vec2 position;
layout(location = 1) in vec3 a_color;
out vec3 vertexColor;
void main() {
    gl_Position = vec4(position, 0.0, 1.0);
    vertexColor = a_color;
};

#shader fragment
#version 330 core
//layout(location = 0) out vec4 color;
out vec3 color;
in vec3 vertexColor;

//uniform vec4 u_Color;

void main() {
    color = vertexColor;
};