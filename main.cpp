#include <iostream>
#include <time.h>
#include "disc.h"
#include "cylinder.h"
#include "plane.h"
#include "phong_shader.h"
#include "constant_shader.h"
#include "ilcontainer.h"
#include "instance.h"

using namespace std;
using namespace glm;

//#define	FULL_SCREEN
#define	MOVE
//#define	SHOW_NORMALS

#ifdef USE_STEREO
#define	DISPLAY_MODE	(GLUT_RGBA | GLUT_DEPTH | GLUT_STEREO)
#else
#define	DISPLAY_MODE	(GLUT_RGBA | GLUT_DEPTH)
#endif // USE_STEREO

vector<Instance> instances;

Disc disc1(64, pi<float>() * 1.5f, 0.25f, 0.125f);
Disc disc2(64, pi<float>() * 2.0f , 0.25f , 0.0f);
Cylinder cylinder(64, 8, pi<float>() * 2.0f, 2.0f, 0.1f);
Plane plane(8 , 8);
vec3 eye(0.0f, 0.0f, 15.0f);
vec3 cop(0.0f, 0.0f, 0.0f);
vec3 up(0.0f, 1.0f, 0.0f);

PhongShader phong_shader;
ConstantShader constant_shader;
vector<ShaderInitializer> shaders;

struct Window
{
	GLuint handle;
	ivec2 size;
	bool wireframe;
	float aspect;
	float current_time;
	float fovy;
	float near_distance;
	float far_distance;
} window;

void ReshapeFunc(int w, int h)
{
	if (h > 0)
	{
		window.size = ivec2(w, h);
		window.aspect = float(w) / float(h);
	}
}

void CloseFunc()
{
	window.handle = BAD_GL_VALUE;
}

void KeyboardFunc(unsigned char c, int x, int y)
{
	switch (c)
	{
		case '+':
			window.fovy++;
			if (window.fovy > 90.0f)
				window.fovy = 90.0f;
			break;

		case '-':
			window.fovy--;
			if (window.fovy < 2.0f)
				window.fovy = 2.0f;
			break;

		case 'w':
			window.wireframe = !window.wireframe;
			glPolygonMode(GL_FRONT_AND_BACK , (window.wireframe ? GL_LINE : GL_FILL));
			break;

		case 'x':
		case 27:
			glutLeaveMainLoop();
			return;
	}
}

void DrawScene()
{
	phong_shader.GLReturnedError("DrawScene() - entering");
#ifdef MOVE
	mat4 m = rotate(mat4() , radians(window.current_time * 30.0f) , vec3(0.0f , 1.0f , 0.2f));
	m = translate(m, vec3(0.0f, 11.5f * cos(window.current_time * 0.5f) + 2.0f, 11.5f * sin(window.current_time * 0.5f) + 2.0f));
#else
	mat4 m;
#endif // MOVE

	mat4 view_matrix = lookAt(vec3(m * vec4(eye, 1.0f)), cop, up);
	mat4 model_matrix;
	mat4 projection_matrix = perspective(radians(window.fovy), window.aspect, window.near_distance, window.far_distance);

	vec3 z_axis = vec3(0.0f, 0.0f, 1.0f);
	vec3 y_axis = vec3(0.0f, 1.0f, 0.0f);
	vec3 ambient = vec3(0.1f, 0.1f, 0.1f);
	vec3 specular = vec3(1.0f, 1.0f, 1.0f);
	float c_offset = radians(45.0f);

	glViewport(0, 0, window.size.x, window.size.y);

	for (unsigned int i = 0; i < instances.size(); i++)
	{
		model_matrix = translate(mat4(), instances[i].position);
		model_matrix = rotate(model_matrix, radians(window.current_time * instances[i].rate) + instances[i].offset, y_axis);
		if (i % 3 != 2)
			model_matrix = rotate(model_matrix, c_offset, z_axis);

		phong_shader.Use(model_matrix, view_matrix, projection_matrix);
		phong_shader.SetMaterial(instances[i].diffuse, specular, 32.0f, ambient);
		phong_shader.SetLightPosition(vec3(0.0f, 0.0f, 1000.0f));
		if (i % 3 == 0)
			disc1.Draw(false);
		else if (i % 3 == 1)
			disc2.Draw(false);
		else
			plane.Draw(false);

		phong_shader.UnUse();

		#ifdef SHOW_NORMALS
		if (i == 0)
		{
			constant_shader.Use(model_matrix, view_matrix, projection_matrix);
			constant_shader.SetMaterial(vec3(0.0f, 0.0f, 0.8f), specular, 128.0f, vec3(1.0f, 0.0f, 0.0f));
			disc1.Draw(true);
			constant_shader.UnUse();
		}
		#endif
	}

	model_matrix = mat4();
	mat4 mz = model_matrix;
	model_matrix = scale(model_matrix, vec3(0.5f, 0.5f, 16.0f));

	phong_shader.Use(model_matrix, view_matrix, projection_matrix);
	phong_shader.SetMaterial(vec3(0.0f, 0.0f, 0.8f), specular, 128.0f, ambient);
	phong_shader.SetLightPosition(vec3(0.0f, 1000.0f, 0.0f));
	cylinder.Draw(false);
	phong_shader.UnUse();

#ifdef SHOW_NORMALS
	constant_shader.Use(model_matrix, view_matrix, projection_matrix);
	constant_shader.SetMaterial(vec3(0.0f, 0.0f, 0.8f), specular, 128.0f, vec3(1.0f, 1.0f, 1.0f));
	cylinder.Draw(true);
	constant_shader.UnUse();
#endif

	model_matrix = rotate(mz, radians(90.0f), y_axis);
	model_matrix = scale(model_matrix, vec3(0.5f, 0.5f, 16.0f));
	phong_shader.Use(model_matrix, view_matrix, projection_matrix);
	phong_shader.SetMaterial(vec3(1.0f, 0.0f, 0.0f), specular, 128.0f, ambient);
	phong_shader.SetLightPosition(vec3(0.0f, 1000.0f, 0.0f));
	cylinder.Draw(false);
	phong_shader.UnUse();

	model_matrix = rotate(mz, radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));
	model_matrix = scale(model_matrix, vec3(0.5f, 0.5f, 16.0f));
	phong_shader.Use(model_matrix, view_matrix, projection_matrix);
	phong_shader.SetMaterial(vec3(0.0f, 1.0f, 0.0f), specular, 128.0f, ambient);
	phong_shader.SetLightPosition(vec3(0.0f, 1000.0f, 0.0f));
	cylinder.Draw(false);
	phong_shader.UnUse();
}

void DisplayFunc()
{
	window.current_time = float(glutGet(GLUT_ELAPSED_TIME)) / 1000.0f;
	vec4 crimson(0.6f, 0.0f, 0.0f, 1.0f);
	#ifdef STEREO
	glDrawBuffer(GL_BACK);
	#endif
	glClearColor(crimson.r, crimson.g, crimson.b, crimson.a);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CW);
	//glEnable(GL_CULL_FACE);
	DrawScene();
	glutSwapBuffers();
}

void IdleFunc()
{
	glutPostRedisplay();
}

int main(int argc, char * argv[])
{
	window.fovy = 50.0f;
	window.near_distance = 0.1f;
	window.far_distance = 50.0f;

	srand(unsigned int(time(NULL)));
	// glutInit must be the first thing to use OpenGL
	glutInit(&argc, argv);
	// Initializes the Develeoper's Imaging Library
	ilInit();
	// Add a line here for every shader defined. This will
	// take care of loading and unloading.
	shaders.push_back(ShaderInitializer(&phong_shader, "per-fragment-phong.vs.glsl", "per-fragment-phong.fs.glsl", "phong shader failed to initialize"));
	shaders.push_back(ShaderInitializer(&constant_shader, "constant.vs.glsl", "constant.fs.glsl", "phong shader failed to initialize"));
	// Adds objects to the world.
	Instance::DefineInstances(instances, 150);

	glutInitWindowSize(512, 512);
	glutInitWindowPosition(-1, -1);
	glutInitDisplayMode(DISPLAY_MODE);

	window.handle = glutCreateWindow("Basic Shape Viewer");

#ifdef FULL_SCREEN
	glutFullScreen();
#endif

	glutReshapeFunc(ReshapeFunc);
	glutCloseFunc(CloseFunc);
	glutDisplayFunc(DisplayFunc);
	glutKeyboardFunc(KeyboardFunc);
	glutIdleFunc(IdleFunc);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

	// This must be called AFTER an OpenGL context has been built.
	if (glewInit() != GLEW_OK)
	{
		cerr << "GLEW failed to initialize." << endl;
		cerr << "Hit enter to exit:";
		cin.get();
		return 0;
	}

	if (!ShaderInitializer::Initialize(&shaders))
		exit(0);

	glutMainLoop();

	ShaderInitializer::TakeDown(&shaders);
	return 0;
}

/*
Correct method
The Toe - in method while
giving workable stereo
pairs is not correct, it
also introduces vertical
parallax which is most
noticeable for objects
in the outer field of
view.The correct method
is to use what is
sometimes known as the
"parallel axis
asymmetric frustum
perspective projection".
In this case the view
vectors for each camera
remain parallel and a
glFrustum() is used to
describe the perspective
projection.
3D Stereo Rendering Using OpenGL(and GLUT) ???4 / 6
http://astronomy.swin.edu.au/~pbourke/opengl/stereogl/ 2005-3-31

	// Misc stuff
	ratio = window.aspect;
	radians = radians(window.fovy);
	wd2 = near * tan(radians);
	ndfl = near / camera.focallength;

	// Clear the buffers
	glDrawBuffer(GL_BACK_LEFT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (stereo) {
		glDrawBuffer(GL_BACK_RIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	if (stereo) {
		mat4 projection_matrix;

		// Derive the two eye positions 
		CROSSPROD(camera.vd, camera.vu, r);
		normalise(r);
		r *= camera.eyesep / 2.0f;

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		float left = -ratio * wd2 - 0.5 * camera.eyesep * ndfl;
		float right = ratio * wd2 - 0.5 * camera.eyesep * ndfl;
		float top = wd2;
		float bottom = -wd2;
		projection_matrix = frustum(left, right, bottom, top, near, far);

		glMatrixMode(GL_MODELVIEW);
		glDrawBuffer(GL_BACK_RIGHT);
		glLoadIdentity();
		gluLookAt(camera.vp.x + r.x, camera.vp.y + r.y, camera.vp.z + r.z,
			camera.vp.x + r.x + camera.vd.x,
			camera.vp.y + r.y + camera.vd.y,
			camera.vp.z + r.z + camera.vd.z,
			camera.vu.x, camera.vu.y, camera.vu.z);
		MakeLighting();
		MakeGeometry();



		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		left = -ratio * wd2 + 0.5 * camera.eyesep * ndfl;
		right = ratio * wd2 + 0.5 * camera.eyesep * ndfl;
		top = wd2;
		bottom = -wd2;
		frustum(left, right, bottom, top, near, far);

		glMatrixMode(GL_MODELVIEW);
		glDrawBuffer(GL_BACK_LEFT);
		glLoadIdentity();
		gluLookAt(camera.vp.x - r.x, camera.vp.y - r.y, camera.vp.z - r.z,
			camera.vp.x - r.x + camera.vd.x,
			camera.vp.y - r.y + camera.vd.y,
			camera.vp.z - r.z + camera.vd.z,
			camera.vu.x, camera.vu.y, camera.vu.z);
		MakeLighting();
		MakeGeometry(); 
*/