#pragma once
#include "shader.h"

// Portions lifted from OpenGL SuperBible 7th Edition.

class ConstantShader : public Shader
{
public:
	void Use(glm::mat4 &model_matrix, glm::mat4 &view_matrix, glm::mat4 &projection_matrix);
	void UnUse();
	void SetMaterial(glm::vec3 diffuse_albedo, glm::vec3 specular_albedo, float specular_power, glm::vec3 ambient);
	void CustomSetup();

private:

	struct
	{
		GLuint mv_matrix;
		GLuint proj_matrix;
		GLuint ambient;
	} uniforms;
};