#version 330 core

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vColor;

out vec3 outColor;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 proj_matrix;

void main()
{
	gl_Position = proj_matrix * view_matrix * model_matrix * vec4(vPos, 1.0);
	outColor = vColor;
}
