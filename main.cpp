#include <iostream>
#include <time.h>
#include "disc.h"
#include "cylinder.h"
#include "phong_shader.h"
#include "constant_shader.h"

using namespace std;
using namespace glm;

#define	MOVE

#ifdef USE_STEREO
#define	DISPLAY_MODE	(GLUT_RGBA | GLUT_DEPTH | GLUT_STEREO)
#else
#define	DISPLAY_MODE	(GLUT_RGBA | GLUT_DEPTH)
#endif // USE_STEREO

inline float Random(float min, float max)
{
	return float(rand()) / float(RAND_MAX) * (max - min) + min;
}

class Instance
{
public:
	Instance(vec3 p, float o, float r, vec3 d) : position(p), offset(o), rate(r), diffuse(d) {};

	vec3 position;
	float offset;
	float rate;
	vec3 diffuse;
};

vector<Instance> instances;

void DefineInstances(int n = 32)
{
	float d = 8.0f;

	for (int i = 0; i < n; i++)
	{
		vec3 diffuse(Random(0.2f, 1.0f), Random(0.2f, 1.0f), Random(0.2f, 1.0f));
		instances.push_back(Instance(
				vec3(Random(-d, d), Random(-d, d), Random(-d, d)), 
				Random(0.0f, pi<float>() * 2.0f), 
				Random(-360.0f, 360.0f), 
				diffuse));
	}
	instances[0].position = vec3();
	instances[0].rate = 60.0f;
}

Disc disc1(32, pi<float>() * 1.5f,  0.25f, 0.125f);
Cylinder cylinder(72, 1, pi<float>() * 2.0f);

string exit_string = "Hit enter to exit:";

PhongShader phong_shader;
ConstantShader constant_shader;

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

void TimerFunc(int param)
{
	if (window.handle != BAD_GL_VALUE)
	{
		glutTimerFunc(param, TimerFunc, param);
		glutPostRedisplay();
	}
}

vec3 eye(0.0f, 0.0f, 12.0f);
vec3 cop(0.0f, 0.0f, 0.0f);
vec3 up(0.0f, 1.0f, 0.0f);

void DrawScene()
{
	phong_shader.GLReturnedError("DrawScene() - entering");
#ifdef MOVE
	mat4 m = rotate(mat4(), radians(window.current_time * 20.0f), vec3(0.0f, 1.0f, 0.0f));
	m = translate(m, vec3(0.0f, 0.0f, 11.5f * cos(window.current_time * 0.5f)));
#else
	mat4 m;
#endif // MOVE

	mat4 view_matrix = lookAt(vec3(m * vec4(eye, 1.0f)), cop, up);
	mat4 model_matrix;
	mat4 projection_matrix = perspective(radians(window.fovy), window.aspect, window.near_distance, window.far_distance);

	vec3 z_axis = vec3(0.0f, 0.0f, 1.0f);
	vec3 y_axis = vec3(0.0f, 1.0f, 0.0f);
	vec3 ambient = vec3(0.0f, 0.0f, 0.0f);
	vec3 specular = vec3(1.0f, 1.0f, 1.0f);
	float c_offset = radians(45.0f);

	glViewport(0, 0, window.size.x, window.size.y);

	for (unsigned int i = 0; i < instances.size(); i++)
	{
		model_matrix = translate(mat4(), instances[i].position);
		model_matrix = rotate(model_matrix, radians(window.current_time * instances[i].rate) + instances[i].offset, y_axis);
		model_matrix = rotate(model_matrix, c_offset, z_axis);

		phong_shader.Use(model_matrix, view_matrix, projection_matrix);
		phong_shader.SetMaterial(instances[i].diffuse, specular, 64.0f, ambient);
		disc1.Draw(false);
		phong_shader.UnUse();
		phong_shader.GLReturnedError("DrawScene() - after first unuse");

		if (i == 0)
		{
			constant_shader.Use(model_matrix, view_matrix, projection_matrix);
			constant_shader.SetMaterial(vec3(1.0f, 0.0f, 0.0f));
			disc1.Draw(true);
			constant_shader.UnUse();
		}
	}

	model_matrix = mat4();
	model_matrix = rotate(model_matrix, radians(window.current_time * instances[0].rate) + instances[0].offset, y_axis);

	phong_shader.Use(model_matrix, view_matrix, projection_matrix);
	phong_shader.SetMaterial(instances[0].diffuse, specular, 64.0f, ambient);
	cylinder.Draw(false);
	phong_shader.UnUse();
	phong_shader.GLReturnedError("DrawScene() - after second unuse");

	constant_shader.Use(model_matrix, view_matrix, projection_matrix);
	constant_shader.SetMaterial(vec3(0.0f, 1.0f, 0.0f));
	cylinder.Draw(true);
	constant_shader.UnUse();


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
	srand(unsigned int(time(NULL)));
	glutInit(&argc, argv);
	glutInitWindowSize(512, 512);
	glutInitWindowPosition(-1, -1);
	glutInitDisplayMode(DISPLAY_MODE);

	window.handle = glutCreateWindow("Basic Shape Viewer");
	window.fovy = 60.0f;
	window.near_distance = 0.1f;
	window.far_distance = 50.0f;

	glutFullScreen();
	glutReshapeFunc(ReshapeFunc);
	glutCloseFunc(CloseFunc);
	glutDisplayFunc(DisplayFunc);
	glutKeyboardFunc(KeyboardFunc);
	glutIdleFunc(IdleFunc);
	//glutTimerFunc(1000 / 60, TimerFunc, 1000 / 60);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

	if (glewInit() != GLEW_OK)
	{
		cerr << "GLEW failed to initialize." << endl;
		cerr << exit_string;
		cin.get();
		return 0;
	}

	if (!phong_shader.Initialize("per-fragment-phong.vs.glsl", "per-fragment-phong.fs.glsl"))
	{
		cerr << "Phong shader failed to initialize." << endl;
		cerr << exit_string;
		cin.get();
		return 0;
	}

	if (!constant_shader.Initialize("constant.vs.glsl", "constant.fs.glsl"))
	{
		cerr << "Phong shader failed to initialize." << endl;
		cerr << exit_string;
		cin.get();
		return 0;
	}

	phong_shader.CustomSetup();
	constant_shader.CustomSetup();

	DefineInstances(10);

	glutMainLoop();
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