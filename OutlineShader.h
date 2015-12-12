#pragma once
#include "shader.h"

class OutlineShader : public Shader
{
public:
	void UnUse();
	void SetMaterial(glm::vec3 diffuse_albedo, glm::vec3 specular_albedo, float specular_power, glm::vec3 ambient) {};
	void CustomSetup();

	static void Outline(OutlineShader & os, FrameBufferObject & fbo, bool copy_back, int image_size, int number_of_samples);


private:

	struct
	{
		GLuint image_size;
		GLuint texture;
	} uniforms;
	int image_size;
};