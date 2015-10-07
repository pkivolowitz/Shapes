#include <iostream>
#include "shape.h"

using namespace glm;
using namespace std;

bool Shape::PostGLInitialize()
{
	return true;
}

void Shape::GLTakeDown()
{
}

bool Shape::CommonGLInitialization()
{
	if (this->vertices.size() == 0)
		return false;

	glGenVertexArrays(1, &this->normal_array_handle);
	glBindVertexArray(normal_array_handle);
	glGenBuffers(1, &this->normal_visualization_handle);
	assert(this->normal_visualization_handle != (GLuint)-1);
	glBindBuffer(GL_ARRAY_BUFFER, this->normal_visualization_handle);
	glBufferData(GL_ARRAY_BUFFER, this->normal_visualization_coordinates.size() * sizeof(glm::vec3), &this->normal_visualization_coordinates[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
	glBindVertexArray(0);

	glGenVertexArrays(1, &vertex_array_handle);
	glBindVertexArray(vertex_array_handle);

	glGenBuffers(1, &this->vertex_values_handle);
	assert(this->vertex_values_handle != (GLuint)-1);
	glBindBuffer(GL_ARRAY_BUFFER, this->vertex_values_handle);
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(glm::vec3), &this->vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

	if (this->normals.size() > 0)
	{
		glGenBuffers(1, &this->normal_values_handle);
		assert(this->normal_values_handle != (GLuint)-1);
		glBindBuffer(GL_ARRAY_BUFFER, this->normal_values_handle);
		glBufferData(GL_ARRAY_BUFFER, this->normals.size() * sizeof(glm::vec3), &this->normals[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_TRUE, 0, (GLubyte *)NULL);
	}

	if (this->colors.size() > 0)
	{
		glGenBuffers(1, &color_values_handle);
		assert(color_values_handle != -1);
		glBindBuffer(GL_ARRAY_BUFFER, color_values_handle);
		glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec4), &colors[0], GL_STATIC_DRAW);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
		glEnableVertexAttribArray(1);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	this->GLReturnedError("Shape::CommonGLInitialization - exiting.");
	return true;
}

bool Shape::GLReturnedError(char * s)
{
	bool return_error = false;
#ifdef DEBUG
	GLenum glerror;

	while ((glerror = glGetError()) != GL_NO_ERROR)
	{
		return_error = true;
		cerr << s << ": " << gluErrorString(glerror) << endl;
	}
#endif
	return return_error;
}
