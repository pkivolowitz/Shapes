#version 410 core

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
uniform float opacity = 1.0;
uniform vec3 ambient;

uniform sampler2D base_texture;

// A global time value as needed by many of the shader toy shaders.
uniform float global_time;

vec3 light_position = vec3(0.0, 0.0, 100.0);

subroutine(color_t)
vec4 Constant()
{
	return vec4(ambient, opacity);
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

	return vec4(ambient + diffuse + specular, opacity);
}

subroutine(color_t)
vec4 PPLWithTexture()
{
	vec4 t = texture(base_texture, fs_in.T);
	vec3 diffuse2 = vec3(t);
	vec3 N2 = fs_in.N;

	if (!gl_FrontFacing)
	{
		N2 = -N2;
	}
	vec3 n = normalize(N2);
	vec3 s = normalize(light_position - fs_in.P);
	vec3 v = normalize(-fs_in.P);
	vec3 r = reflect(-s, n);
	vec3 diffuse = max(dot(s, n), 0.0) * diffuse2;
	vec3 specular = pow(max(dot(r, v), 0.0), specular_power) * specular_albedo;

	return vec4(vec3(ambient + diffuse + specular), t.a * opacity);
			//vec4(fs_in.T.t, fs_in.T.t, fs_in.T.t, 0); // 
}

subroutine(color_t)
vec4 ShaderToy1()
{
	// http://www.pouet.net/prod.php?which=57245

	vec3 c;
	float l;
	float z= global_time;

	for(int i=0;i<3;i++)
	{
		vec2 p = fs_in.T;
		vec2 uv = p;
		p -= 0.5;
		z +=.07;
		l = length(p);
		uv += p / l * (sin(z)+1.0) * abs(sin(l * 9.0 - z * 2.0));
		c[i] = 0.01/length(abs(mod(uv, 1.0) - 0.5));
	}
	return vec4(c / l, 1.0);
}

void main(void)
{
	FragColor = ColorMode();
}

