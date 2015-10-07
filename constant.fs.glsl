#version 410 core

// Output
layout (location = 0) out vec4 color;

uniform vec3 ambient;

void main(void)
{
	color = vec4(ambient, 1.0);
}
