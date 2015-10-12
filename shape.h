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

class Shape
{
public:
	class ShapeInfo
	{
	public:
		ShapeInfo(GLuint c, GLuint h, void * d, GLuint s, GLuint ne, GLuint t, GLuint n) :
			channel_number(c), handle(h), data(d),
			sizeof_element(s), number_of_elements(ne), type(t),
			number_of_type(n)
		{
		}

		GLuint channel_number;
		GLuint handle;
		void * data;
		GLuint sizeof_element;
		GLuint number_of_elements;
		GLuint type;
		GLuint number_of_type;
	};

	enum
	{
		VERTICES = 0,
		COLORS,
		NORMALS,
		TEXTURE_COORDINATES,
		LAST_VALUE
	};

	Shape() {}

	virtual void Draw(bool draw_normals = false) = 0;

protected:
	std::vector<ShapeInfo> si;
	virtual bool CommonGLInitialization();
	virtual bool PreGLInitialize() = 0;
	virtual void NonGLTakeDown() = 0;
	virtual bool PostGLInitialize();
	virtual void GLTakeDown();

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec4> colors;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> textures;
	std::vector<GLuint> indices;
	std::vector<glm::vec3> normal_visualization_coordinates;

	GLuint vertex_array_handle;
	GLuint index_array_handle;

	GLuint normal_array_handle;
	GLuint normal_visualization_handle;

	bool GLReturnedError(char * s);

	glm::vec4 RandomColor(glm::vec4 & previous_color, float min = -0.1f , float max = 0.1f);

	inline float Random(float min , float max)
	{
		return (float(rand()) / float(RAND_MAX)) * (max - min) + min;
	}

};
