#version 410 core

// Lifted largely from OpenGL Superbible 7th Edition

// Per-vertex attributes. The location here corresponds
// to the attribute number in the calling CPU-based program.
layout (location = 0) in vec4 position;
layout (location = 2) in vec3 normal;

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
    vec3 L;
    vec3 V;
} vs_out;

// Position of light is taken to be already in eye coordinates.
uniform vec3 light_pos = vec3(0.0, 0.0, 100.0);

void main(void)
{
	// Calculate view-space coordinate
	// Results in P being in eye coordinates.
	vec4 position_in_eye_coordinates = mv_matrix * position;

	// Calculate light vector
	vs_out.L = normalize(light_pos - vec3(position_in_eye_coordinates));

	// Calculate normal in view-space
	vs_out.N = normalize(inverse(transpose(mat3(mv_matrix))) * normal);

	// Calculate view vector
	vs_out.V = normalize(-position_in_eye_coordinates.xyz);

	// Calculate the clip-space position of each vertex
	// Recall, position_in_eye_coordinates has been multiplied by the mv.
	gl_Position = proj_matrix * position_in_eye_coordinates;
}
