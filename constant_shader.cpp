#include <iostream>
#include <stdexcept>
#include <assert.h>

#include "constant_shader.h"

using namespace std;
using namespace glm;

#ifndef BAD_GL_VALUE
#define	BAD_GL_VALUE	(GLuint(-1))
#endif // !BAD_GL_VALUE

void ConstantShader::Use(mat4 &model_matrix, mat4 &view_matrix, mat4 &projection_matrix)
{
	this->GLReturnedError("ConstantShader::Use() - entering");
	Shader::Use();
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, this->uniforms_buffer);
	uniforms_block * block = (uniforms_block *) glMapBufferRange(GL_UNIFORM_BUFFER, 0, sizeof(uniforms_block), GL_MAP_WRITE_BIT);
	block->mv_matrix = view_matrix * model_matrix;
	block->proj_matrix = projection_matrix;
//	glUnmapBuffer(GL_UNIFORM_BUFFER);
	this->GLReturnedError("ConstantShader::Use() - exiting");
}

void ConstantShader::UnUse()
{
	this->GLReturnedError("ConstantShader::UnUse() - entering");
	glUnmapBuffer(GL_UNIFORM_BUFFER);
	Shader::UnUse();
	this->GLReturnedError("ConstantShader::UnUse() - exiting");
}

void ConstantShader::CustomSetup()
{
	glGenBuffers(1, &this->uniforms_buffer);
	glBindBuffer(GL_UNIFORM_BUFFER, this->uniforms_buffer);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(uniforms_block), NULL, GL_DYNAMIC_DRAW);

	Shader::Use();
	uniforms.ambient = glGetUniformLocation(this->program_id, "ambient");
	if (uniforms.ambient == BAD_GL_VALUE)
		throw std::exception("one or more of the uniforms in the phong shader was not found");
	Shader::UnUse();
}

void ConstantShader::SetMaterial(glm::vec3 diffuse_albedo, glm::vec3 specular_albedo, float specular_power, glm::vec3 ambient)
{
	assert(this->is_used == true);
	glUniform3fv(uniforms.ambient, 1, (GLfloat *)(&ambient));
}
