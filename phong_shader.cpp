#include <iostream>
#include <stdexcept>
#include <assert.h>

#include "phong_shader.h"

using namespace std;
using namespace glm;

#ifndef BAD_GL_VALUE
#define	BAD_GL_VALUE	(GLuint(-1))
#endif // !BAD_GL_VALUE

void PhongShader::Use(mat4 &model_matrix, mat4 &view_matrix, mat4 &projection_matrix)
{
	this->GLReturnedError("PhongShader::Use() - entering");
	Shader::Use();
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, this->uniforms_buffer);
	uniforms_block * block = (uniforms_block *) glMapBufferRange(GL_UNIFORM_BUFFER, 0, sizeof(uniforms_block), GL_MAP_WRITE_BIT);
	block->mv_matrix = view_matrix * model_matrix;
	block->view_matrix = view_matrix;
	block->proj_matrix = projection_matrix;
	block->normal_matrix = inverse(transpose(mat3(block->mv_matrix)));
//	glUnmapBuffer(GL_UNIFORM_BUFFER);
	this->GLReturnedError("PhongShader::Use() - exiting");
}

void PhongShader::UnUse()
{
	this->GLReturnedError("PhongShader::UnUse() - entering");

	glUnmapBuffer(GL_UNIFORM_BUFFER);
	Shader::UnUse();

	this->GLReturnedError("PhongShader::UnUse() - exiting");
}

void PhongShader::SetLightPosition(glm::vec3 light_position)
{
	assert(this->is_used == true);
	glUniform3fv(uniforms.light_position, 1, (GLfloat *)(&light_position));
}

void PhongShader::CustomSetup()
{
	glGenBuffers(1, &this->uniforms_buffer);
	glBindBuffer(GL_UNIFORM_BUFFER, this->uniforms_buffer);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(uniforms_block), NULL, GL_DYNAMIC_DRAW);

	Shader::Use();
	uniforms.light_position = glGetUniformLocation(this->program_id, "light_position");
	uniforms.diffuse_albedo = glGetUniformLocation(this->program_id, "diffuse_albedo");
	uniforms.specular_albedo = glGetUniformLocation(this->program_id, "specular_albedo");
	uniforms.specular_power = glGetUniformLocation(this->program_id, "specular_power");
	uniforms.ambient = glGetUniformLocation(this->program_id, "ambient");
	if (uniforms.diffuse_albedo == BAD_GL_VALUE || uniforms.specular_albedo == BAD_GL_VALUE || uniforms.specular_power == BAD_GL_VALUE || uniforms.ambient == BAD_GL_VALUE)
		throw std::exception("one or more of the uniforms in the phong shader was not found");

	Shader::UnUse();
}

void PhongShader::SetMaterial(glm::vec3 diffuse_albedo, glm::vec3 specular_albedo, float specular_power, glm::vec3 ambient)
{
	assert(this->is_used == true);
	glUniform1f(uniforms.specular_power, specular_power);
	glUniform3fv(uniforms.specular_albedo, 1, (GLfloat *)(&specular_albedo));
	glUniform3fv(uniforms.diffuse_albedo, 1, (GLfloat *)(&diffuse_albedo));
	glUniform3fv(uniforms.ambient, 1, (GLfloat *)(&ambient));
}
