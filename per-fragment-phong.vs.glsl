#version 410 core

// Lifted largely from OpenGL Superbible 7th Edition

// Per-vertex attributes. The location here corresponds
// to the attribute number in the calling CPU-based program.
layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec2 textures;

// Matrices we'll need - these are passed an uniforms...
// Uniforms are constants within the shader program.
layout (std140) uniform constants
{
    mat4 mv_matrix;
    mat4 view_matrix;
    mat4 proj_matrix;
	mat3 normal_matrix;
};

// These are outputs of the vertex shader. They will
// arrive as inputs to the fragment shader after having
// been rasterized. This syntax (in particular) bundles
// the enclosed outs into what amounts to a struct.
out VS_OUT
{
    vec3 N;
    vec3 P;
	vec4 C;
	vec2 T;
} vs_out;


void main(void)
{
	vs_out.N = normalize(inverse(transpose(mat3(mv_matrix))) * normal);
	vs_out.P = vec3(mv_matrix * vec4(position, 1.0));
	vs_out.C = color;
	vs_out.T = textures;

	gl_Position = proj_matrix * mv_matrix * vec4(position, 1.0);
}
