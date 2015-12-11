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

uniform sampler2D base_texture;

// A global time value as needed by many of the shader toy shaders.
uniform float global_time;
float iGlobalTime = global_time;

//****************************************************************************************
//Fire Shader from ShaderToy Methods
//https://www.shadertoy.com/view/MdX3zr

float noise(vec3 p) //Thx to Las^Mercury
{
	vec3 i = floor(p);
	vec4 a = dot(i, vec3(1., 57., 21.)) + vec4(0., 57., 21., 78.);
	vec3 f = cos((p-i)*acos(-1.))*(-.5)+.5;
	a = mix(sin(cos(a)*a),sin(cos(1.+a)*(1.+a)), f.x);
	a.xy = mix(a.xz, a.yw, f.y);
	return mix(a.x, a.y, f.z);
}

float sphere(vec3 p, vec4 spr)
{
	return length(spr.xyz-p) - spr.w;
}

float flame(vec3 p)
{
	float d = sphere(p*vec3(1.,.5,1.), vec4(.0,-1.,.0,1.));
	return d + (noise(p+vec3(.0,iGlobalTime*2.,.0)) + noise(p*3.)*.5)*.25*(p.y) ;
}

float scene(vec3 p)
{
	return min(100.-length(p) , abs(flame(p)) );
}

vec4 raymarch(vec3 org, vec3 dir)
{
	float d = 0.0, glow = 0.0, eps = 0.02;
	vec3  p = org;
	bool glowed = false;
	
	for(int i=0; i<64; i++)
	{
		d = scene(p) + eps;
		p += d * dir;
		if( d>eps )
		{
			if(flame(p) < .0)
				glowed=true;
			if(glowed)
       			glow = float(i)/64.;
		}
	}
	return vec4(p,glow);
}

//*************************************************************************
//Noise Blur Shader from ShaderToy Methods
//https://www.shadertoy.com/view/4dlGDN
// Created by inigo quilez - iq/2013
// License Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.

float hash( float n )
{
    return fract(sin(n)*43758.5453);
}

float blurNoise( in vec2 x )
{
    vec2 p = floor(x);
    vec2 f = fract(x);
    f = f*f*(3.0-2.0*f);
    float n = p.x + p.y*57.0;
    return mix(mix( hash(n+  0.0), hash(n+  1.0),f.x),
               mix( hash(n+ 57.0), hash(n+ 58.0),f.x),f.y);
}

vec2 map( vec2 p, in float offset )
{
	p.x += 0.1*sin( iGlobalTime + 2.0*p.y ) ;
	p.y += 0.1*sin( iGlobalTime + 2.0*p.x ) ;
	
	float a = blurNoise(p*1.5 + sin(0.1*iGlobalTime))*6.2831;
	a -= offset;
	return vec2( cos(a), sin(a) );
}

//**************************************************************************
//Fractal-Like Shader from ShaderToy Site
//https://www.shadertoy.com/view/Mdf3z7
#define MaxSteps 30
#define MinimumDistance 0.0009
#define normalDistance     0.0002

#define Iterations 7
#define PI 3.141592
#define Scale 3.0
#define FieldOfView 1.0
#define Jitter 0.05
#define FudgeFactor 0.7
#define NonLinearPerspective 2.0
#define DebugNonlinearPerspective false

#define Ambient 0.32184
#define Diffuse 0.5
#define LightDir vec3(1.0)
#define LightColor vec3(1.0,1.0,0.858824)
#define LightDir2 vec3(1.0,-1.0,1.0)
#define LightColor2 vec3(0.0,0.333333,1.0)
#define Offset vec3(0.92858,0.92858,0.32858)

vec2 rotate(vec2 v, float a) {
	return vec2(cos(a)*v.x + sin(a)*v.y, -sin(a)*v.x + cos(a)*v.y);
}

// Two light sources. No specular 
vec3 getLight(in vec3 color, in vec3 normal, in vec3 dir) {
	vec3 lightDir = normalize(LightDir);
	float diffuse = max(0.0,dot(-normal, lightDir)); // Lambertian
	
	vec3 lightDir2 = normalize(LightDir2);
	float diffuse2 = max(0.0,dot(-normal, lightDir2)); // Lambertian
	
	return
	(diffuse*Diffuse)*(LightColor*color) +
	(diffuse2*Diffuse)*(LightColor2*color);
}

//Fractal-Like Shader from ShaderToy Site Methods
// DE: Infinitely tiled Menger IFS.
//
// For more info on KIFS, see:
// http://www.fractalforums.com/3d-fractal-generation/kaleidoscopic-%28escape-time-ifs%29/
float DE(in vec3 z)
{
	// enable this to debug the non-linear perspective
	if (DebugNonlinearPerspective) {
		z = fract(z);
		float d=length(z.xy-vec2(0.5));
		d = min(d, length(z.xz-vec2(0.5)));
		d = min(d, length(z.yz-vec2(0.5)));
		return d-0.01;
	}
	// Folding 'tiling' of 3D space;
	z  = abs(1.0-mod(z,2.0));

	float d = 1000.0;
	for (int n = 0; n < Iterations; n++) {
		z.xy = rotate(z.xy,4.0+2.0*cos( iGlobalTime/8.0));		
		z = abs(z);
		if (z.x<z.y){ z.xy = z.yx;}
		if (z.x< z.z){ z.xz = z.zx;}
		if (z.y<z.z){ z.yz = z.zy;}
		z = Scale*z-Offset*(Scale-1.0);
		if( z.z<-0.5*Offset.z*(Scale-1.0))  z.z+=Offset.z*(Scale-1.0);
		d = min(d, length(z) * pow(Scale, float(-n)-1.0));
	}
	
	return d-0.001;
}

// Finite difference normal
vec3 getNormal(in vec3 pos) {
	vec3 e = vec3(0.0,normalDistance,0.0);
	
	return normalize(vec3(
			DE(pos+e.yxx)-DE(pos-e.yxx),
			DE(pos+e.xyx)-DE(pos-e.xyx),
			DE(pos+e.xxy)-DE(pos-e.xxy)
			)
		);
}

// Solid color 
vec3 getColor(vec3 normal, vec3 pos) {
	return vec3(1.0);
}


// Pseudo-random number
// From: lumina.sourceforge.net/Tutorials/Noise.html
float rand(vec2 co){
	return fract(cos(dot(co,vec2(4.898,7.23))) * 23421.631);
}

vec4 rayMarch(in vec3 from, in vec3 dir, in vec2 fragCoord) {
	// Add some noise to prevent banding
	float totalDistance = Jitter*rand(fragCoord.xy+vec2(iGlobalTime));
	vec3 dir2 = dir;
	float distance;
	int steps = 0;
	vec3 pos;
	for (int i=0; i < MaxSteps; i++) {
		// Non-linear perspective applied here.
		dir.zy = rotate(dir2.zy,totalDistance*cos( iGlobalTime/4.0)*NonLinearPerspective);
		
		pos = from + totalDistance * dir;
		distance = DE(pos)*FudgeFactor;
		totalDistance += distance;
		if (distance < MinimumDistance) break;
		steps = i;
	}
	
	// 'AO' is based on number of steps.
	// Try to smooth the count, to combat banding.
	float smoothStep =   float(steps) + distance/MinimumDistance;
	float ao = 1.1-smoothStep/float(MaxSteps);
	
	// Since our distance field is not signed,
	// backstep when calc'ing normal
	vec3 normal = getNormal(pos-dir*normalDistance*3.0);
	
	vec3 color = getColor(normal, pos);
	vec3 light = getLight(color, normal, dir);
	color = (color*Ambient+light)*ao;
	return vec4(color,1.0);
}

//**************************************************************************
//Subroutines to run the different shaders

//https://www.shadertoy.com/view/XlfGRj
subroutine(color_t)
vec4 spaceShader()
{
	// Star Nest by Pablo Román Andrioli
	// This content is under the MIT License.

	#define iterations 17
	#define formuparam 0.53

	#define volsteps 20
	#define stepsize 0.1

	#define zoom   0.800
	#define tile   0.850
	#define speed  0.010 

	#define brightness 0.0015
	#define darkmatter 0.300
	#define distfading 0.730
	#define saturation 0.850

	//get coords and direction
	vec2 uv=fs_in.T; //fragCoord.xy/iResolution.xy-.5;
	uv.y*=1.0; //iResolution.y/iResolution.x;
	vec3 dir=vec3(uv*zoom,1.);
	float time=iGlobalTime*speed+.25;

	////mouse rotation
	//float a1=.5+iMouse.x/iResolution.x*2.;
	//float a2=.8+iMouse.y/iResolution.y*2.;
	//mat2 rot1=mat2(cos(a1),sin(a1),-sin(a1),cos(a1));
	//mat2 rot2=mat2(cos(a2),sin(a2),-sin(a2),cos(a2));
	//dir.xz*=rot1;
	//dir.xy*=rot2;
	vec3 from=vec3(1.,.5,0.5);
	from+=vec3(time*2.,time,-2.);
	//from.xz*=rot1;
	//from.xy*=rot2;
	
	//volumetric rendering
	float s=0.1,fade=1.;
	vec3 v=vec3(0.);
	for (int r=0; r<volsteps; r++) {
		vec3 p=from+s*dir*.5;
		p = abs(vec3(tile)-mod(p,vec3(tile*2.))); // tiling fold
		float pa,a=pa=0.;
		for (int i=0; i<iterations; i++) { 
			p=abs(p)/dot(p,p)-formuparam; // the magic formula
			a+=abs(length(p)-pa); // absolute sum of average change
			pa=length(p);
		}
		float dm=max(0.,darkmatter-a*a*.001); //dark matter
		a*=a*a; // add contrast
		if (r>6) fade*=1.-dm; // dark matter, don't render near
		//v+=vec3(dm,dm*.5,0.);
		v+=fade;
		v+=vec3(s,s*s,s*s*s*s)*a*brightness*fade; // coloring based on distance
		fade*=distfading; // distance fading
		s+=stepsize;
	}
	v=mix(vec3(length(v)),v,saturation); //color adjust

	return vec4(v*.01,1.);	
	
}

subroutine(color_t)
vec4 fireShader()
{
	vec2 v = fs_in.T; //-1.0 + 2.0 * fragCoord.xy / iResolution.xy;
	v.x *= 1.0; //iResolution.x/iResolution.y;
	v.y = 1.0 - v.y; //flip the flame
	
	vec3 org = vec3(-4., 0., 8.);
	vec3 dir = normalize(vec3(v.x*1.6, -v.y, -1.5));
	
	vec4 p = raymarch(org, dir);
	float glow = p.w;
	
	vec4 col = mix(vec4(1.,.5,.1,1.), vec4(0.1,.5,1.,1.), p.y*.02+.4);
	
	return mix(vec4(0.), col, pow(glow*2.,4.));
	//FragColor = mix(vec4(1.), mix(vec4(1.,.5,.1,1.),vec4(0.1,.5,1.,1.),p.y*.02+.4), pow(glow*2.,4.));
}

subroutine(color_t)
vec4 noiseShader()
{
	vec2 p = fs_in.T; //fragCoord.xy / iResolution.xy;
	vec2 uv = -1.0 + 2.0*p;
	uv.x *= 1.0; //iResolution.x / iResolution.y;
		
    float offset = iGlobalTime + fs_in.T.x; //fragCoord.x/iResolution.x;
    
	float acc = 0.0;
	vec3  col = vec3(0.0);
	for( int i=0; i<32; i++ )
	{
		vec2 dir = map( uv, offset );
		
		float h = float(i)/32.0;
		float w = 4.0*h*(1.0-h);
		
		vec3 ttt = w*texture2D( base_texture, uv ).xyz;
		ttt *= mix( vec3(0.6,0.7,0.7), vec3(1.0,0.95,0.9), 0.5 - 0.5*dot( reflect(vec3(dir,0.0), vec3(1.0,0.0,0.0)).xy, vec2(0.707) ) );
		col += w*ttt;
		acc += w;
		
		uv += 0.008*dir;
	}
	col /= acc;
    
	float gg = dot( col, vec3(0.333) );
	vec3 nor = normalize( vec3( dFdx(gg), 0.5, dFdy(gg) ) );
	col += vec3(0.4)*dot( nor, vec3(0.7,0.01,0.7) );

	vec2 di = map( uv, offset );
	col *= 0.65 + 0.35*dot( di, vec2(0.707) );
	col *= 0.20 + 0.80*pow( 4.0*p.x*(1.0-p.x), 0.1 );
	col *= 1.7;

	return vec4( col, 1.0 );
}

subroutine(color_t)
vec4 fractalShader()
{
	// Camera position (eye), and camera target
	vec3 camPos = 0.5*iGlobalTime*vec3(1.0,0.0,0.0);
	vec3 target = camPos + vec3(1.0,0.0*cos(iGlobalTime),0.0*sin(0.4*iGlobalTime));
	vec3 camUp  = vec3(0.0,1.0,0.0);
	
	// Calculate orthonormal camera reference system
	vec3 camDir   = normalize(target-camPos); // direction for center ray
	camUp = normalize(camUp-dot(camDir,camUp)*camDir); // orthogonalize
	vec3 camRight = normalize(cross(camDir,camUp));
	
	vec2 coord =-1.0+2.0*fs_in.T; //fragCoord.xy/iResolution.xy;
	coord.x *= 1.0; //iResolution.x/iResolution.y;
	
	// Get direction for this pixel
	vec3 rayDir = normalize(camDir + (coord.x*camRight + coord.y*camUp)*FieldOfView);
	
	return rayMarch(camPos, rayDir, fs_in.T );
}

void main(void)
{
	FragColor = ColorMode();
}