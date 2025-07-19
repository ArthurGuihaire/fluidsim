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
    vec4 centerTexel = texture(inputTexture, TexCoords);
    vec2 pressureForce = vec2(
        texture(inputTexture, TexCoords - vec2(texelSize[0], 0)).w
         - texture(inputTexture, TexCoords + vec2(texelSize[0], 0)).w, 
        texture(inputTexture, TexCoords - vec2(0, texelSize[1])).w
         - texture(inputTexture, TexCoords + vec2(0, texelSize[1])).w) / 5;
    FragColor = vec4(centerTexel.xy + pressureForce, centerTexel.zw);
}
