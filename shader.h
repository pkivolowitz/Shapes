#pragma once

/*
A shader support BASE class.

Perry Kivolowitz - University of Wisconsin - Madison
Computer Sciences Department

Created:	2/25/13
Updates:	10/4/2015		Rolling into generic shape builders.

*/

#include <iostream>
#include <sstream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
	Shader();
	void TakeDown();
	void Use();
	void UnUse();
	virtual bool Initialize(char * vertex_shader_file, char * fragment_shader_file);
	virtual void CustomSetup() = 0;

	GLuint vertex_shader_id;
	GLuint fragment_shader_id;
	GLuint program_id;
	bool LoadShader(const char * file_name, GLuint shader_id);
	std::stringstream GetShaderLog(GLuint shader_id);
	bool GLReturnedError(char * s);

protected:
	bool is_used;

private:
};
