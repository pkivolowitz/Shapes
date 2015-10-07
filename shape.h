#pragma once

#include <vector>
#include <assert.h>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>

#ifndef BAD_GL_VALUE
#define	BAD_GL_VALUE	(GLuint(-1))
#endif // !BAD_GL_VALUE

/*	Attribute	Attribute
				Number
	vertices	0
	colors		1
	normals		2
*/

class Shape
{
public:
	Shape() : vertex_values_handle(BAD_GL_VALUE),
		color_values_handle(BAD_GL_VALUE),
		normal_values_handle(BAD_GL_VALUE),
		vertex_array_handle(BAD_GL_VALUE)
	{}

	virtual void Draw(bool draw_normals = false) = 0;

protected:
	virtual bool CommonGLInitialization();
	virtual bool PreGLInitialize() = 0;
	virtual void NonGLTakeDown() = 0;
	virtual bool PostGLInitialize();
	virtual void GLTakeDown();

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec4> colors;
	std::vector<glm::vec3> normals;
	std::vector<GLuint> indices;
	std::vector<glm::vec3> normal_visualization_coordinates;

	GLuint vertex_values_handle;
	GLuint color_values_handle;
	GLuint normal_values_handle;
	GLuint vertex_array_handle;

	GLuint normal_array_handle;
	GLuint normal_visualization_handle;

	bool GLReturnedError(char * s);

	glm::vec3 RandomColor(float min = 0.0f , float max = 1.0);

	inline float Random(float min , float max)
	{
		return (float(rand()) / float(RAND_MAX)) * (max - min) + min;
	}

};
