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
	glUniformMatrix4fv(uniforms.mv_matrix , 1 , GL_FALSE , value_ptr(view_matrix * model_matrix));
	glUniformMatrix4fv(uniforms.proj_matrix , 1 , GL_FALSE , value_ptr(projection_matrix));
	this->GLReturnedError("ConstantShader::Use() - exiting");
}

void ConstantShader::UnUse()
{
	this->GLReturnedError("ConstantShader::UnUse() - entering");
	Shader::UnUse();
	this->GLReturnedError("ConstantShader::UnUse() - exiting");
}

void ConstantShader::CustomSetup()
{
	Shader::Use();
	uniforms.ambient = glGetUniformLocation(this->program_id, "ambient");
	uniforms.mv_matrix = glGetUniformLocation(this->program_id , "mv_matrix");
	uniforms.proj_matrix = glGetUniformLocation(this->program_id , "proj_matrix");
	if (uniforms.ambient == BAD_GL_VALUE || uniforms.proj_matrix == BAD_GL_VALUE || uniforms.mv_matrix == BAD_GL_VALUE)
		throw std::exception("one or more of the uniforms in the phong shader was not found");
	Shader::UnUse();
}

void ConstantShader::SetMaterial(glm::vec3 diffuse_albedo, glm::vec3 specular_albedo, float specular_power, glm::vec3 ambient)
{
	assert(this->is_used == true);
	glUniform3fv(uniforms.ambient, 1, (GLfloat *)(&ambient));
}
