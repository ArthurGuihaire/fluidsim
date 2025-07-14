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
    vec4 texel = texture(inputTexture, TexCoords);
    vec2 texelLeft = texture(inputTexture, TexCoords - vec2(texelSize[0], 0)).xy;
    vec2 texelRight = texture(inputTexture, TexCoords + vec2(texelSize[0], 0)).xy;
    vec2 texelDown = texture(inputTexture, TexCoords - vec2(0, texelSize[1])).xy;
    vec2 texelUp = texture(inputTexture, TexCoords + vec2(0, texelSize[1])).xy;
    vec2 blurred_velocity = (texelLeft + texelRight + texelDown + texelUp + texel.xy) * 0.2;

    FragColor = vec4(blurred_velocity, texel.zw);
}
