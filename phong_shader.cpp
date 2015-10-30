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

	// TO DO - TRY USING THE OLD WAY OF GETTING TO UNIFORMS - THIS SEEMS TO BE SLOWING ME DOWN.
	mat4 modelview_matrix = view_matrix * model_matrix;
	mat3 normal_matrix = normal_matrix = inverse(transpose(mat3(modelview_matrix)));

	glUniformMatrix4fv(this->uniforms.modelview_matrix, 1, GL_FALSE, value_ptr(modelview_matrix));
	glUniformMatrix4fv(this->uniforms.view_matrix , 1 , GL_FALSE , value_ptr(view_matrix));
	glUniformMatrix4fv(this->uniforms.projection_matrix , 1 , GL_FALSE , value_ptr(projection_matrix));
	glUniformMatrix3fv(this->uniforms.normal_matrix , 1 , GL_FALSE , value_ptr(normal_matrix));

	this->GLReturnedError("PhongShader::Use() - exiting");
}

void PhongShader::UnUse()
{
	this->GLReturnedError("PhongShader::UnUse() - entering");
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
	Shader::Use();
	uniforms.light_position = glGetUniformLocation(this->program_id, "light_position");
	uniforms.diffuse_albedo = glGetUniformLocation(this->program_id, "diffuse_albedo");
	uniforms.specular_albedo = glGetUniformLocation(this->program_id, "specular_albedo");
	uniforms.specular_power = glGetUniformLocation(this->program_id, "specular_power");
	uniforms.ambient = glGetUniformLocation(this->program_id, "ambient");
	uniforms.modelview_matrix = glGetUniformLocation(this->program_id , "mv_matrix");
	uniforms.view_matrix = glGetUniformLocation(this->program_id , "view_matrix");
	uniforms.normal_matrix = glGetUniformLocation(this->program_id , "normal_matrix");
	uniforms.projection_matrix = glGetUniformLocation(this->program_id , "proj_matrix");
	//	if (uniforms.diffuse_albedo == BAD_GL_VALUE || uniforms.specular_albedo == BAD_GL_VALUE || uniforms.specular_power == BAD_GL_VALUE || uniforms.ambient == BAD_GL_VALUE)
//		throw std::exception("one or more of the uniforms in the phong shader was not found");

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
