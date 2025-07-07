#shader vertex
#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0); 
    TexCoords = aTexCoords;
}  

#shader fragment
#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
uniform sampler2D inputTexture;
uniform vec2 texelSize;

void main()
{
    vec2 vel = vec2((texture(inputTexture, TexCoords).x * 2 - 1) * texelSize[0], (texture(inputTexture, TexCoords).y * 2 - 1) * texelSize[1]);
    FragColor = texture(inputTexture, TexCoords - vel);
    //FragColor = vec3(texture(inputTexture, TexCoords).x, texture(inputTexture, TexCoords).y, texture(inputTexture, TexCoords).z);
}
