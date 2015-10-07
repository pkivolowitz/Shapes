#version 410 core

// Lifted largely from OpenGL Superbible 7th Edition

// Output
layout (location = 0) out vec4 color;

// Input from vertex shader - this must correspond exactly
// to the outputs of the vertex shader. The semantics of
// these are the same, however the values have been rasterized
// across the face of what ever it is we're drawing now.

in VS_OUT
{
    vec3 N;
    vec3 L;
    vec3 V;
} fs_in;

// Material properties
uniform vec3 diffuse_albedo;
uniform vec3 specular_albedo;
uniform float specular_power;
uniform vec3 ambient;

void main(void)
{
//	if (gl_FrontFacing)
//	{
		// Normalize the incoming N, L and V vectors
		vec3 N = normalize(fs_in.N);
		vec3 L = normalize(fs_in.L);
		vec3 V = normalize(fs_in.V);

		// Calculate R locally
		vec3 R = reflect(-L, N);

		// Compute the diffuse and specular components for each fragment
		vec3 diffuse = max(dot(N, L), 0.0) * diffuse_albedo;
		vec3 specular = pow(max(dot(R, V), 0.0), specular_power) * specular_albedo;

		// Write final color to the framebuffer
		color = vec4(ambient + diffuse + specular, 1.0);
//	}
//	else
//	{
//		color = vec4(ambient, 1.0);
//	}
}
