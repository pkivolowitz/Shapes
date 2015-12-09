#include <iostream>
#include <stdexcept>
#include <assert.h>

#include "sarah-malorie-shader.h"

using namespace std;
using namespace glm;

#ifndef BAD_GL_VALUE
#define	BAD_GL_VALUE	(GLuint(-1))
#endif // !BAD_GL_VALUE

void SarahMalorieShader::Use(mat4 &model_matrix, mat4 &view_matrix, mat4 &projection_matrix)
{
	this->GLReturnedError("SarahMalorieShader::Use() - entering");
	Shader::Use();

	// Note - this originally used uniform blocks mapped to local memory.
	// This caused an insane amount of performance hits. This old style of using
	// glUniformMatrix4fv gained a performance increase of about three orders of
	// magnitude with lots of shapes.

	mat4 modelview_matrix = view_matrix * model_matrix;
	mat3 normal_matrix = normal_matrix = inverse(transpose(mat3(modelview_matrix)));

	glUniformMatrix4fv(this->uniforms.modelview_matrix, 1, GL_FALSE, value_ptr(modelview_matrix));
	glUniformMatrix4fv(this->uniforms.view_matrix, 1, GL_FALSE, value_ptr(view_matrix));
	glUniformMatrix4fv(this->uniforms.projection_matrix, 1, GL_FALSE, value_ptr(projection_matrix));
	glUniformMatrix3fv(this->uniforms.normal_matrix, 1, GL_FALSE, value_ptr(normal_matrix));

	this->GLReturnedError("SarahMalorieShader::Use() - exiting");
}

void SarahMalorieShader::UnUse()
{
	assert(this->is_used == true);
	this->GLReturnedError("SarahMalorieShader::UnUse() - entering");
	Shader::UnUse();
	this->GLReturnedError("SarahMalorieShader::UnUse() - exiting");
}

void SarahMalorieShader::EnableTexture(ILContainer & ilcontainer, GLuint texture_unit)
{
	assert(this->is_used == true);
	glUniform1i(this->uniforms.base_texture_location, texture_unit);
	ilcontainer.Bind(texture_unit);
	glEnable(GL_TEXTURE_2D);
}

void SarahMalorieShader::SelectSubroutine(int subroutine_index)
{
	assert(this->is_used == true);
	if (subroutine_index < 0 || subroutine_index >= int(this->subroutine_indices.size()))
		throw std::invalid_argument("SarahMalorieShader::SelectSubroutine() - bad index");

	glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, (const GLuint *)&this->subroutine_indices[subroutine_index]);
}

void SarahMalorieShader::SetLightPosition(glm::vec3 light_position)
{
	assert(this->is_used == true);
	glUniform3fv(uniforms.light_position, 1, (GLfloat *)(&light_position));
}

void SarahMalorieShader::SetGlobalTime(float global_time)
{
	assert(this->is_used == true);
	glUniform1f(uniforms.global_time, global_time);
}

void SarahMalorieShader::SetSaturationValue(float value)
{
	assert(this->is_used == true);
	glUniform1f(uniforms.saturation_value, value);
}

void SarahMalorieShader::CustomSetup()
{
	Shader::Use();
	uniforms.light_position = glGetUniformLocation(this->program_id, "light_position");
	uniforms.diffuse_albedo = glGetUniformLocation(this->program_id, "diffuse_albedo");
	uniforms.specular_albedo = glGetUniformLocation(this->program_id, "specular_albedo");
	uniforms.specular_power = glGetUniformLocation(this->program_id, "specular_power");
	uniforms.ambient = glGetUniformLocation(this->program_id, "ambient");
	uniforms.modelview_matrix = glGetUniformLocation(this->program_id, "mv_matrix");
	uniforms.view_matrix = glGetUniformLocation(this->program_id, "view_matrix");
	uniforms.normal_matrix = glGetUniformLocation(this->program_id, "normal_matrix");
	uniforms.projection_matrix = glGetUniformLocation(this->program_id, "proj_matrix");
	uniforms.base_texture_location = glGetUniformLocation(this->program_id, "base_texture");
	uniforms.global_time = glGetUniformLocation(this->program_id, "global_time");
	uniforms.saturation_value = glGetUniformLocation(this->program_id, "saturation");
	this->subroutine_indices.push_back(glGetSubroutineIndex(this->program_id, GL_FRAGMENT_SHADER, "Constant"));
	this->subroutine_indices.push_back(glGetSubroutineIndex(this->program_id, GL_FRAGMENT_SHADER, "PerPixelLighting"));
	this->subroutine_indices.push_back(glGetSubroutineIndex(this->program_id, GL_FRAGMENT_SHADER, "PPLWithTexture"));
	this->subroutine_indices.push_back(glGetSubroutineIndex(this->program_id, GL_FRAGMENT_SHADER, "ShaderToy1"));
	this->subroutine_indices.push_back(glGetSubroutineIndex(this->program_id, GL_FRAGMENT_SHADER, "ShaderToy2"));
	this->subroutine_indices.push_back(glGetSubroutineIndex(this->program_id, GL_FRAGMENT_SHADER, "ShaderToy3"));
	this->subroutine_indices.push_back(glGetSubroutineIndex(this->program_id, GL_FRAGMENT_SHADER, "ShaderToy4"));
	this->subroutine_indices.push_back(glGetSubroutineIndex(this->program_id, GL_FRAGMENT_SHADER, "ShaderToy5"));
	this->subroutine_indices.push_back(glGetSubroutineIndex(this->program_id, GL_FRAGMENT_SHADER, "SaturationShader"));
	Shader::UnUse();
}

void SarahMalorieShader::SetMaterial(glm::vec3 diffuse_albedo, glm::vec3 specular_albedo, float specular_power, glm::vec3 ambient)
{
	assert(this->is_used == true);
	glUniform1f(uniforms.specular_power, specular_power);
	glUniform3fv(uniforms.specular_albedo, 1, (GLfloat *)(&specular_albedo));
	glUniform3fv(uniforms.diffuse_albedo, 1, (GLfloat *)(&diffuse_albedo));
	glUniform3fv(uniforms.ambient, 1, (GLfloat *)(&ambient));
}
