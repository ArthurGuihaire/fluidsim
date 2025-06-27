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
//uniform float texelSize;

void main()
{
    float texelSizeX = 0.00125;
    float texelSizeY = 0.00167;
    vec2 vel = vec2((texture(inputTexture, TexCoords).x / 256.0f - 0.5) * texelSizeX, (texture(inputTexture, TexCoords).x / 256.0f - 0.5) * texelSizeY);
    FragColor = texture(inputTexture, TexCoords + vel);
}
