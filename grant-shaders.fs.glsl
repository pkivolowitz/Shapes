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
uniform vec3 ambient;

uniform sampler2D base_texture;
uniform float band_heights[16];

// A global time value as needed by many of the shader toy shaders.
uniform float global_time;

vec3 light_position = vec3(0.0, 0.0, 100.0);

const float RAINBOW_SPLINE_SIZE = 6.0;
subroutine(color_t)
vec4 Constant()
{
	return vec4(ambient, 1.0);
}

subroutine(color_t)
vec4 Spectrum()
{	
	vec3 texture = vec3(texture(base_texture, fs_in.T)); // Really weird things happen with this turned on
	vec2 p = fs_in.T;
	vec3 result;
	float x_pos_in_hex = floor(p.x * 16.);

	//Color
	vec3 white = vec3((1. + sin(global_time + x_pos_in_hex)),
			  (1. + asin(global_time + x_pos_in_hex)),
			  (1. + cos(global_time + x_pos_in_hex)));

	//Band
	int xPosition_16 = int(p.x * 16);

	//Height
	float restraint = band_heights[xPosition_16];
	//restraint *= 2.;
	if (p.y > 1 - restraint)
	{
		result = texture * white;
	}
	else
	{
		result = texture;
	}
	
	return vec4( result, 1.0 );
}

/**
 * Creates a hashkey based on a 3D variable.
 * Note: Using haskeys directly as noise function gives non-coherent noise.
 * @return: Haskey in range [0.0, 1.0)
 */
float hash3(in vec3 p){
    // Transform 3D parameter into a 1D value:
    // Note: higher value means 'higher frequency' when plugging uv coordinates.
    float h = dot(p, vec3(123.45, 678.91, 234.56));
    
    // Use a sinusoid function to create both positive and negative numbers.
    // Multiply by a big enough number and then taking only the fractional part creates a pseudo-random value.
    return fract(cos(h)*12345.6789);
}

/**
 * Create a coherent noise using the perline noise algorithm. Haskeys are
 * used to remove the need of an array of random values.
 * @return: noise value in the range[0.0, 1.0)
 */
float perlinNoise3( in vec3 p )
{
    // see: http://webstaff.itn.liu.se/~stegu/TNM022-2005/perlinnoiselinks/perlin-noise-math-faq.html#whatsnoise
    vec3 i = floor(p); // Use hashing with this to fake a gridbased value noise.
    vec3 f = fract(p);
	
    // Using this 'ease curve' generates more visually pleasing noise then without.
    // Function describes a function similar to a smoothstep.
	vec3 u = f*f*(3.0-2.0*f);

    float dx1 = mix(hash3(i + vec3(0.0,0.0,0.0)), 
                    hash3(i + vec3(1.0,0.0,0.0)), u.x);
    float dx2 = mix(hash3(i + vec3(0.0,1.0,0.0)), 
                    hash3(i + vec3(1.0,1.0,0.0)), u.x);
    float dy1 = mix(dx1, dx2, u.y);
    
    float dx3 = mix(hash3(i + vec3(0.0,0.0,1.0)), 
                    hash3(i + vec3(1.0,0.0,1.0)), u.x);
    float dx4 = mix(hash3(i + vec3(0.0,1.0,1.0)), 
                    hash3(i + vec3(1.0,1.0,1.0)), u.x);
    float dy2 = mix(dx3, dx4, u.y);
    
    return mix(dy1, dy2, u.z);
}


/**
 * Performs a fractal sum of the same noise function for different 'frequencies'.
 * @return: noise value in the range [0.0, ~1.94/2)
 */
float fractalSumNoise3(in vec3 p){
    float value = 0.0;
    
    float f = 1.0;
    
    // Experimentation yielded 5 itterations gave optimal results. Less itterations gave too
    // blotchy result, and more itterations did no longer have any significant visual impact.
    for (int i = 0; i < 5; i++){
        value += perlinNoise3(p * f)/f;
        f = f * 2.0;
    }
    
    return value/2.0;
}

float pattern( in vec3 p )
  {
      vec3 q = vec3( fractalSumNoise3( p + vec3(0.0,0.0,0.0)),
                     fractalSumNoise3( p + vec3(5.2,1.3,0.7)),
                     fractalSumNoise3( p + vec3(6.7,2.6,1.2)));

      return fractalSumNoise3( p + 4.0*q );
  }


subroutine(color_t)
vec4 WaterShader()
{
	//Water turbulence effect by joltz0r 2013-07-04, improved 2013-07-07
	//https://www.shadertoy.com/view/MdlXz8
    float time = global_time * .5+23.0;
    // uv should be the 0-1 uv of texture...
    vec2 uv = fs_in.T;
   

    vec2 p = mod(uv * 6.28318530718, 6.28318530718)-250.0;
    vec2 i = vec2(p);
    float c = 1.0;
    float inten = .005;

    for (int n = 0; n < 5; n++)
    {
        float t = time * (1.0 - (3.5 / float(n+1)));
        i = p + vec2(cos(t - i.x) + sin(t + i.y), sin(t - i.y) + cos(t + i.x));
        c += 1.0 / length(vec2(p.x / (sin(i.x + t) / inten), p.y / (cos(i.y + t) / inten)));
    }
    c /= float(5);
    c = 1.17-pow(c, 1.4);
    vec3 colour = vec3(pow(abs(c), 8.0));
    colour = clamp(colour + vec3(0.0, 0.35, 0.5), 0.0, 1.0);
   
    return vec4(colour / 1, 1.0);
}

subroutine(color_t)
vec4 PaintShader()
{
	//From Shadertoy.com: https://www.shadertoy.com/view/llX3Rn
	vec2 uv = fs_in.T;

	return vec4(pattern(vec3(5.0*uv,0.5+0.5*sin(0.3*global_time))),
                        pattern(vec3(5.0*uv,0.5+0.5*cos(0.3*global_time))),
                        pattern(vec3(0.5+0.5*sin(0.3*global_time),5.0*uv)),
                        1.0);

}
void main(void)
{
	FragColor = ColorMode();
}

