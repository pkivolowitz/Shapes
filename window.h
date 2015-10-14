#pragma once
#include <vector>
#include "GL/glew.h"
#include "GL/freeglut.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>

#ifndef BAD_GL_VALUE
#define	BAD_GL_VALUE	(GLuint(-1))
#endif // !BAD_GL_VALUE

class Window
{
public:
	Window(char * window_name, void(*DisplayFunc)() , void(*KeyboardFunc)(unsigned char c , int x , int y) , void(*ReshapeFunc)(int w , int h), void (*CloseFunc)(), glm::ivec2 size, float fovy, float near_distance, float far_distance);

	static Window * FindCurrentWindow(std::vector<Window> & windows);
	static void PostAllRedisplays(std::vector<Window> & windows);

	static float current_time;
	void(*DisplayFunc)();
	void(*KeyboardFunc)(unsigned char c , int x , int y);
	void(*ReshapeFunc)(int w , int h);
	void(*CloseFunc)();

	char * window_name;
	GLuint handle;
	glm::ivec2 size;
	bool wireframe;
	float aspect;
	float fovy;
	float near_distance;
	float far_distance;
};