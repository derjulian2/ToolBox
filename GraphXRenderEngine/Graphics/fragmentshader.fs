#version 330 core
out vec4 FragColor;

in vec4 vertexColor;
in vec2 vertexTexCoords;

uniform sampler2D vertexTexture;

void main()
{
    FragColor = vertexColor * texture(vertexTexture, vertexTexCoords);
} 