#version 450 core
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexColor;

out vec3 fColor;
uniform mat4 mvp;
void main()
{
	fColor = vertexColor;

	gl_Position = mvp * vec4(vertexPosition, 1.0f);;
}