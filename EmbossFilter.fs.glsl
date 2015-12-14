#version 420 core

//#define PROCESSING_TEXTURE_SHADER

layout(location = 0) out vec4 FragColor;
layout(binding = 0) uniform sampler2D texture;

in vec2 TexCoord;

vec2 texOffset = vec2(0.01, 0.01);

const vec4 lumcoeff = vec4(0.299, 0.587, 0.114, 0);

void main() {
  vec2 tc0 = TexCoord.st + vec2(-texOffset.s, -texOffset.t);
  vec2 tc1 = TexCoord.st + vec2(         0.0, -texOffset.t);
  vec2 tc2 = TexCoord.st + vec2(-texOffset.s,          0.0);
  vec2 tc3 = TexCoord.st + vec2(+texOffset.s,          0.0);
  vec2 tc4 = TexCoord.st + vec2(         0.0, +texOffset.t);
  vec2 tc5 = TexCoord.st + vec2(+texOffset.s, +texOffset.t);
  
  vec4 col0 = texture2D(texture, tc0);
  vec4 col1 = texture2D(texture, tc1);
  vec4 col2 = texture2D(texture, tc2);
  vec4 col3 = texture2D(texture, tc3);
  vec4 col4 = texture2D(texture, tc4);
  vec4 col5 = texture2D(texture, tc5);

  vec4 sum = vec4(0.5) + (col0 + col1 + col2) - (col3 + col4 + col5);
  float lum = dot(sum, lumcoeff);

  FragColor = vec4(lum, lum, lum, 1.0); /** vec4(1.0, 0.0, 0.0, 1.0);*/  
}