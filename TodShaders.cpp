#include <iostream>
#include <stdexcept>
#include <assert.h>

#include "TodShaders.h"

using namespace std;
using namespace glm;

#ifndef BAD_GL_VALUE
#define	BAD_GL_VALUE	(GLuint(-1))
#endif // !BAD_GL_VALUE

void TodShaders::Use(mat4 &model_matrix, mat4 &view_matrix, mat4 &projection_matrix)
{
	this->GLReturnedError("TodShaders::Use() - entering");
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

	this->GLReturnedError("TodShaders::Use() - exiting");
}

void TodShaders::UnUse()
{
	assert(this->is_used == true);
	this->GLReturnedError("TodShaders::UnUse() - entering");
	Shader::UnUse();
	this->GLReturnedError("TodShaders::UnUse() - exiting");
}

void TodShaders::EnableTexture(ILContainer & ilcontainer, GLuint texture_unit)
{
	assert(this->is_used == true);
	glUniform1i(this->uniforms.base_texture_location, texture_unit);
	ilcontainer.Bind(texture_unit);
	glEnable(GL_TEXTURE_2D);
}

void TodShaders::SelectSubroutine(int subroutine_index)
{
	assert(this->is_used == true);
	if (subroutine_index < 0 || subroutine_index >= int(this->subroutine_indices.size()))
		throw std::invalid_argument("TodShaders::SelectSubroutine() - bad index");

	glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, (const GLuint *)&this->subroutine_indices[subroutine_index]);
}

void TodShaders::SetGlobalTime(float global_time)
{
	assert(this->is_used == true);
	glUniform1f(uniforms.global_time, global_time);
}

void TodShaders::CustomSetup()
{
	Shader::Use();
	uniforms.modelview_matrix = glGetUniformLocation(this->program_id, "mv_matrix");
	uniforms.view_matrix = glGetUniformLocation(this->program_id, "view_matrix");
	uniforms.normal_matrix = glGetUniformLocation(this->program_id, "normal_matrix");
	uniforms.projection_matrix = glGetUniformLocation(this->program_id, "proj_matrix");
	uniforms.base_texture_location = glGetUniformLocation(this->program_id, "base_texture");
	uniforms.global_time = glGetUniformLocation(this->program_id, "global_time");
	this->subroutine_indices.push_back(glGetSubroutineIndex(this->program_id, GL_FRAGMENT_SHADER, "spaceShader"));
	this->subroutine_indices.push_back(glGetSubroutineIndex(this->program_id, GL_FRAGMENT_SHADER, "fireShader"));
	this->subroutine_indices.push_back(glGetSubroutineIndex(this->program_id, GL_FRAGMENT_SHADER, "noiseShader"));
	this->subroutine_indices.push_back(glGetSubroutineIndex(this->program_id, GL_FRAGMENT_SHADER, "fractalShader"));
	Shader::UnUse();
}
