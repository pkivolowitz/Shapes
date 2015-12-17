//add to per-fragment-phong

subroutine float floating(vec3 p);
subroutine uniform floating Float;

subroutine(color_t)
vec4 ShaderToy3()
{
	vec2 fragCoord = fs_in.T.xy;
	vec3 COLOR1 = vec3(0.0, 0.0, 0.3);
	vec3 COLOR2 = vec3(0.5, 0.0, 0.0);
	float BLOCK_WIDTH = 0.01;
	vec2 uv = fragCoord.xy;// / fs_in.P.xy;
	
	// To create the BG pattern
	vec3 final_color = vec3(1.0);
	vec3 bg_color = vec3(0.0);
	vec3 wave_color = vec3(0.0);
	
	float c1 = mod(uv.x, 2.0 * BLOCK_WIDTH);
	c1 = step(BLOCK_WIDTH, c1);
	
	float c2 = mod(uv.y, 2.0 * BLOCK_WIDTH);
	c2 = step(BLOCK_WIDTH, c2);
	
	bg_color = mix(uv.x * COLOR1, uv.y * COLOR2, c1 * c2);
	
	
	// To create the waves
	float wave_width = 0.01;
	uv  = -1.0 + 2.0 * uv;
	uv.y += 0.1;
	for(float i = 0.0; i < 10.0; i++) {
		
		uv.y += (0.07 * sin(uv.x + i/7.0 + global_time ));
		wave_width = abs(1.0 / (150.0 * uv.y));
		wave_color += vec3(wave_width * 1.9, wave_width, wave_width * 1.5);
	}
	
	final_color = bg_color + wave_color;
	
	
	return vec4(final_color, 1.0);
}

subroutine(floating)
float f(vec3 p) 
{ 
	p.z += global_time;
	return length(0.05 * cos(9.0 * p.y * p.x) + cos(p) - 0.1 * cos(9.0 * (p.z + 0.3 * p.x - p.y))) - 1.0; 
}
subroutine(color_t)
vec4 ShaderToy2()
{
// Created by inigo quilez - iq/2013
// License Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.

vec2 p = vec2(fs_in.T.x, fs_in.T.y);
vec3 d = 0.5 - vec3(p, 1);
vec3 o = d;
for(int i = 0; i < 99; i++)
{
	o += f(o) * d;
}
return vec4(abs(f(o - d) * vec3(0, 0.1, 0.2) + f(o - 0.6) * vec3(0.2, 0.1, 0)) * (10.0 - o.z), 1);	
}

subroutine(color_t)
vec4 Sepia()
{
	vec3 diffuse2 = vec3(texture(base_texture, fs_in.T));
	vec4 retVal = vec4(diffuse2.x, diffuse2.y, diffuse2.z, 1.0);
    retVal.r = dot(diffuse2, vec3(.393, .769, .189));
    retVal.g = dot(diffuse2, vec3(.349, .686, .168));
    retVal.b = dot(diffuse2, vec3(.272, .534, .131)); 
	return retVal;
}