#pragma once
#include "shader.h"

// Portions lifted from OpenGL SuperBible 7th Edition.

class ConstantShader : public Shader
{
public:
	void Use(glm::mat4 &model_matrix, glm::mat4 &view_matrix, glm::mat4 &projection_matrix);
	void UnUse();
	void SetMaterial(glm::vec3 & ambient);
	void CustomSetup();

private:
	GLuint uniforms_buffer;

	struct uniforms_block
	{
		glm::mat4 mv_matrix;
		glm::mat4 proj_matrix;
	};

	struct
	{
		GLuint ambient;
	} uniforms;
};