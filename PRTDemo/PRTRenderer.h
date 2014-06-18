#ifndef _GLWindow_H_
#define _GLWindow_H_
#include "global.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ShaderProgram.h"
#include "Object.h"
#include "Scene.h"
#include "Light.h"
using glm::vec3;
using glm::mat4;

class PRTRenderer
{
public:
	//GLWindow(void);
	PRTRenderer(int _width = 480, int _height = 480);
	~PRTRenderer(void);
	void initGLFW();
	void initGL();
	void compileShader(const char* vertFileName, const char* fragFileName);
	//void renderPRT(Object* model, Color* light, Color** coeffs, int bands);
	void setUniform();
	void clean();
	void changeMatrics();

	void render2(Scene& scene);
	void renderModel(Object* model);
	void precomputeColor(Scene& scene, Light& light);
	void renderSceneWithLight(Scene& scene, Light& light);

	ShaderProgram prog;

private:
	int width;
	int height;
	mat4 modelMatrix;
	mat4 viewMatrix;

	GLuint vertexArrayID;
	GLuint vertexbuffer;
	GLuint normalbuffer;

	GLFWwindow* window;

	// GLFW callback
	static void error_callback(int error, const char* description);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void cursor_position_callback(GLFWwindow* window, double x, double y);
	static void scroll_callback(GLFWwindow* window, double x, double y);
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

	// display control
	static bool bAnim;
	static bool locked;
	static GLfloat alpha;	// polar angles for light rotation - theta
	static GLfloat beta;	// ploar angles for light rotation - phi
	static GLfloat zoom;
	static int cursorX;
	static int cursorY;

	static LIGHTING_TYPE lightingType;
};

#endif