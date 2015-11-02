#version 410 core
//#extension GL_ARB_explicit_uniform_location : enable

// Output
layout (location = 0) out vec4 FragColor;

subroutine vec4 color_t();
subroutine uniform color_t ColorMode;

// Input from vertex shader - this must correspond exactly
// to the outputs of the vertex shader. The semantics of
// these are the same, however the values have been rasterized
// across the face of what ever it is we're drawing now.

in VS_OUT
{
    vec3 N;
    vec3 P;
	vec4 C;
	vec2 T;
} fs_in;

// Material properties
uniform vec3 diffuse_albedo;
uniform vec3 specular_albedo;
uniform float specular_power;
uniform vec3 ambient;

// Default position of light is taken to be in eye coordinates.
uniform vec3 light_position = vec3(0.0, 0.0, 1000.0);

uniform sampler2D base_texture;

subroutine(color_t)
vec4 Constant()
{
	return vec4(ambient, 1.0);
}

subroutine(color_t)
vec4 PerPixelLighting()
{
	vec3 diffuse2 = diffuse_albedo;
	vec3 N2 = fs_in.N;

	if (!gl_FrontFacing)
	{
		diffuse2 = vec3(fs_in.C);
		N2 = -N2;
	}
	vec3 n = normalize(N2);
	vec3 s = normalize(light_position - fs_in.P);
	vec3 v = normalize(-fs_in.P);
	vec3 r = reflect(-s, n);
	vec3 diffuse = max(dot(s, n), 0.0) * diffuse2;
	vec3 specular = pow(max(dot(r, v), 0.0), specular_power) * specular_albedo;

	return vec4(ambient + diffuse + specular, 1.0);
}

subroutine(color_t)
vec4 PPLWithTexture()
{
	vec3 diffuse2 = vec3(texture(base_texture, fs_in.T));
	vec3 N2 = fs_in.N;

	if (!gl_FrontFacing)
	{
		diffuse2 = vec3(fs_in.C);
		N2 = -N2;
	}
	vec3 n = normalize(N2);
	vec3 s = normalize(light_position - fs_in.P);
	vec3 v = normalize(-fs_in.P);
	vec3 r = reflect(-s, n);
	vec3 diffuse = max(dot(s, n), 0.0) * diffuse2;
	vec3 specular = pow(max(dot(r, v), 0.0), specular_power) * specular_albedo;

	return vec4(ambient + diffuse + specular, 1.0);
}

void main(void)
{
	FragColor = ColorMode();
}
