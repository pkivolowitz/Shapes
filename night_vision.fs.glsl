#version 410


layout (location = 0) 
out vec4 FragColor;


in vec2 TexCoord;

uniform sampler2D source_texture;

uniform int image_size;

vec4 get_pixel(in vec2 coords) {
 return texture2D(source_texture,coords);
}

void main()
{
  //vec3 diffuse2 = vec3(texture(base_texture, fs_in.T));
  vec4 color = texture2D(source_texture, TexCoord);
  float lum = dot(vec4(0.30, 0.59, 0.11, 1.0),color);
  if (lum < 0.2)
      color *= 4.0;
  
  vec4 visionColor = vec4(0.1, 0.95, 0.2,1.0);
  gl_FragColor = color * visionColor;
}