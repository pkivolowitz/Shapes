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

// A global time value as needed by many of the shader toy shaders.
uniform float global_time;

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
		N2 = -N2;
	}
	vec3 n = normalize(N2);
	vec3 s = normalize(light_position - fs_in.P);
	vec3 v = normalize(-fs_in.P);
	vec3 r = reflect(-s, n);
	vec3 diffuse = max(dot(s, n), 0.0) * diffuse2;
	vec3 specular = pow(max(dot(r, v), 0.0), specular_power) * specular_albedo;

	return vec4(ambient + diffuse + specular, 1.0);
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

const int iterations = 12;
const float view = 40.;
//#define CIRCLE
#define COLOR

subroutine(color_t)
vec4 ShaderToy2()
{
	vec2 uv = fs_in.T - .5;
    uv *= view;
    
    for (int i = 0; i < iterations; i++) {
        float ang = atan(uv.y + cos(uv.x * 2.0 + global_time) * 0.5,
                         uv.x + sin(uv.y * 2.0 + global_time) * 0.5) - length(uv) * 0.1;
        float sn = sin(ang);
        float cs = cos(ang);
        
        mat2 m = mat2(sn,cs,-cs,sn);
        uv = uv * 0.2 - abs(uv * 0.5 * m);
    }
    
    #ifdef CIRCLE
    float d = length(mod(uv, 1.0) - 0.5) - 0.4;
    #else
    float d = length(max(abs(mod(uv, 1.0) - 0.5) - vec2(0.1, 0.4), 0.0));
    #endif
    
    #ifdef COLOR
    d += global_time * 0.05;
    d *= 50.0;
    return vec4( sin(d), cos(d + 0.5346), -sin(d + 1.63), 1.0 ) * 0.5 + 0.5;
	#else
    return vec4(max(0.0, 1.0 - d * 100.0));
    #endif
}

#define PI 3.14159265359
subroutine(color_t)
vec4 ShaderToy3()
{
	vec2 uv = fs_in.T;
    float x = uv.x;
	float y = 0.5 + 0.5 * sin(x * global_time * 2.0 * PI) * cos(x * 2.0 * PI);
    vec3 color = vec3(0.0);
    
    float pct = smoothstep( y - 0.02, y, uv.y) - smoothstep( y, y + 0.02, uv.y);
    color = (1.0 - pct) * color + pct * vec3(0.0, 1.0, 0.0);

    return vec4(color,1.0);
}

subroutine(color_t)
vec4 ShaderToy4()
{
	vec2 st = fs_in.T;
    vec3 color = vec3(0.0);

	st *= 5.0;
    st.y += step(1.0, mod(st.x, 2.0)) * global_time;
    st.y += step(1.0, mod(st.y + 1.0, 2.0)) * -global_time;
	st = fract(st);
    
	vec2 size = vec2(0.5) - vec2(0.9) * 0.5;
    vec2 uv = smoothstep(size, size + vec2(0.0001), st);
    uv *= smoothstep(size, size + vec2(0.0001), vec2(1.0) - st);

    color = vec3(uv.x * uv.y);
    color = vec3(atan(st.x), cos(st.x + st.y), atan(st.x + st.y));

    return vec4(color, 1.0);
}

#define mmix 50.0
#define tau 6.2831853

subroutine(color_t)
vec4 ShaderToy5()
{
	vec4 retVal = vec4(0.0, 0.0, 0.0, 1.0);
	float t = -global_time * 3.0;
    vec2 uv = fs_in.T / 2.0;
    float dist = length(uv) / 5.0 * (0.5 + 0.5 * cos(t / 10.0));
    float om = atan(uv.y, uv.x) + dist;
    for (float i = 0.0; i < mmix; i++)
	{
        float omeg = om + PI / 2.0 / mmix * i;
	    vec4 m =  omeg + t + tau * vec4(0, 1, 2, 0) / 3.0;
        m = mod( PI + PI / 2.0 - m, tau) - PI;
       retVal += (0.5 - 1.0 / tau * m ) / mmix;
    }

	return retVal;
}

//The qualifier highp is used to specify the highest available precision for a variable.
in highp vec2 CordTextures;

//The qualifier lowp is used to specify the lowest available precision for a variable.
//0 to 1
uniform lowp float saturation; 

//The qualifier mediump is used to specify a precision between the highest and lowest available precision for a variable.
//stores our color weighting for our luminance extraction.
const mediump vec3 luminanceWeighting = vec3(0.2125, 0.7154, 0.0721);

subroutine(color_t)
vec4 SaturationShader()
{
	/*https://www.objc.io/issues/21-camera-and-photos/gpu-accelerated-image-processing/#more-complex-shader-examples*/

	vec4 textureColor = texture2D(base_texture, CordTextures);
	//figures out the overall luminance of the pixel.
	lowp float luminance = dot(textureColor.rgb, luminanceWeighting);
	//contains the luminance for all three values.
	vec3 greyScaleColor = vec3(luminance);

	//Takes the grayscale color, combines it with the initial texture color, and basing the ratio of the mix on the information we are getting back about the saturation level.
	vec3 mixResult = mix(greyScaleColor, vec3(textureColor), saturation);
	vec4 retVal = vec4(mixResult, textureColor.w);
	return retVal;
}

void main(void)
{
	FragColor = ColorMode();
}