#version 410 core

// Per-vertex inputs
layout (location = 0) in vec4 position;

// Matrices we'll need
uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

void main(void)
{
	gl_Position = proj_matrix * mv_matrix * position;
}
