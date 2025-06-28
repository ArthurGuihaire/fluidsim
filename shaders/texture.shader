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
uniform float texelSize;

void main()
{
    float texelSizeX = texelSize;
    float texelSizeY = texelSize;
    float red = texture(inputTexture, vec2(TexCoords.x + texelSizeX, TexCoords.y + texelSizeY)).x;
    float green = texture(inputTexture, vec2(TexCoords.x - texelSizeX, TexCoords.y + texelSizeY)).y;
    float blue = texture(inputTexture, vec2(TexCoords.x + texelSizeX, TexCoords.y - texelSizeY)).z;
    FragColor = vec4(red + 0.01 * green - 0.01 * blue, green + 0.01 * blue - 0.01 * red, blue + 0.01 * red - 0.01 * green, 0.0);
    //FragColor = (texture(inputTexture, vec2(TexCoords) + vec2(texelSizeX, texelSizeY)) + texture(inputTexture, vec2(TexCoords) + vec2(texelSizeX, 0)) + texture(inputTexture, vec2(TexCoords) + vec2(texelSizeX, -texelSizeY)) + texture(inputTexture, vec2(TexCoords) + vec2(0, -texelSizeY)) + texture(inputTexture, vec2(TexCoords) + vec2(0, texelSizeY)) + texture(inputTexture, vec2(TexCoords) + vec2(-texelSizeX, -texelSizeY)) + texture(inputTexture, vec2(TexCoords) + vec2(-texelSizeX, 0)) + texture(inputTexture, vec2(TexCoords) + vec2(-texelSizeX, texelSizeY))) / 7.9;
}
