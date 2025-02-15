#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aCol;
layout (location = 2) in vec2 aTex;

out vec4 vertexColor;
out vec2 vertexTexCoords;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

void main()
{
	gl_Position = projection_matrix * view_matrix * model_matrix * vec4(aPos, 1.0);
	vertexColor = aCol;
	vertexTexCoords = aTex;
}