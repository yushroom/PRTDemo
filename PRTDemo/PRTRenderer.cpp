#include "PRTRenderer.h"

bool PRTRenderer::bAnim = false;
GLfloat PRTRenderer::alpha = 0.f;
GLfloat PRTRenderer::beta = 0.f;
GLfloat PRTRenderer::zoom = 0.f;
bool PRTRenderer::locked = false;
int PRTRenderer::cursorX = 0;
int PRTRenderer::cursorY = 0;
LIGHTING_TYPE PRTRenderer::lightingType = LIGHTING_TYPE_SH_UNSHADOWED;

PRTRenderer::PRTRenderer(int _width, int _height) 
	: width(_width), height(_height), prog()
{
	initGLFW();

	// Initialize GLEW
	if (GLEW_OK != glewInit()) {
		printf("[ERROR] Failed to initialize GLEW\n");
		exit(EXIT_FAILURE);
	}

	initGL();
	//compileShader();
	//setUniform();
};

PRTRenderer::~PRTRenderer(void)
{
}

void PRTRenderer::initGLFW()
{
    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
		printf("[ERROR] initialize glfw failed!\n");
        exit(EXIT_FAILURE);
	}

    window = glfwCreateWindow(width, height, "PRT Demo", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    //glfwGetFramebufferSize(window, &width, &height);
    framebuffer_size_callback(window, width, height);
}

void PRTRenderer::initGL()
{
	/* Enable smooth shading */
	glShadeModel( GL_SMOOTH );
	/* Set the background black */
	glClearColor( 0.1f, 0.1f, 0.1f, 0.0f );
	/* Depth buffer setup */
	glClearDepth( 1.0f );
	/* Enables Depth Testing */
	glEnable( GL_DEPTH_TEST );
	/* The Type Of Depth Test To Do */
	glDepthFunc( GL_LEQUAL );
	/* Really Nice Perspective Calculations */
	glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

	modelMatrix = mat4(1.0f);
	viewMatrix = glm::lookAt(vec3(0.0f,0.0f,10.0f), vec3(0.0f,0.0f,-10.0f), vec3(0.0f,1.0f,0.0f));
}

void PRTRenderer::compileShader(const char* vertFileName, const char* fragFileName)
{

	if( ! prog.compileShaderFromFile(vertFileName,GLSLShader::VERTEX) ) {
		printf("[ERROT] Vertex shader failed to compile!\n%s", prog.log().c_str());
		exit(1);
	}
	if( ! prog.compileShaderFromFile(fragFileName, GLSLShader::FRAGMENT)) {
		printf("[ERROT] Fragment shader failed to compile!\n%s", prog.log().c_str());
		exit(1);
	}

	prog.bindAttribLocation(0, "VertexPosition");
	prog.bindAttribLocation(1, "VertexNormal");

	if( ! prog.link() ) {
		printf("[ERROT] Shader program failed to link!\n%s", prog.log().c_str());
		exit(1);
	}
	if( ! prog.validate() ) {
		printf("[ERROR] Program failed to validate!\n%s", prog.log().c_str());
		exit(1);
	}
	prog.use();
}

void PRTRenderer::changeMatrics()
{
	this->modelMatrix = glm::rotate(this->modelMatrix, glm::radians(-30.f), vec3(0.0f,1.0f,0.0f));
	mat4 mv = viewMatrix * modelMatrix;
	glfwGetFramebufferSize(window, &width, &height);
	mat4 projection = glm::perspective(45.0f, float(width)/height, 0.1f, 1000.0f);
	prog.setUniform("MVP", projection * mv);
	//prog.setUniform("ModelViewMatrix", mv);
	//prog.setUniform("NormalMatrix",mat3( vec3(mv[0]), vec3(mv[1]), vec3(mv[2]) ));
}

void PRTRenderer::setUniform()
{

	glfwGetFramebufferSize(window, &width, &height);
	mat4 projection = glm::perspective(45.0f, float(width)/height, 0.1f,1000.0f);
	mat4 mv = viewMatrix * modelMatrix;

	//prog.setUniform("Kd", 0.9f, 0.9f, 0.9f);
	//prog.setUniform("Ld", 0.5f, 0.5f, 0.5f);
	//prog.setUniform("LightPosition", viewMatrix * vec4(-5.0f, 220.0f, 215.0f, 1.0f) );
	//prog.setUniform("ModelViewMatrix", mv);
	//prog.setUniform("NormalMatrix",mat3( vec3(mv[0]), vec3(mv[1]), vec3(mv[2]) ));
	prog.setUniform("MVP", projection * mv);
}


void PRTRenderer::renderModel(Object* model)
{
	while (!glfwWindowShouldClose(window)) {
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);

		if (bAnim) changeMatrics();
		if (NULL != model)
			model->render();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}


void PRTRenderer::precomputeColor(Scene& scene, Light& light)
{
	//LIGHTING_TYPE lightingType = LIGHTING_TYPE_SH_UNSHADOWED; 
	//LIGHTING_TYPE lightingType = LIGHTING_TYPE_SH_UNSHADOWED; 
	//LIGHTING_TYPE lightingType = LIGHTING_TYPE_SH_SHADOWED; 
	//Calculate the lit color of each vertex
	int numObjects = scene.object.size();

	for(int i=0; i < numObjects; ++i)
	{
		Object* currentObject = scene.object[i];
		int numVertices = currentObject->vertices.size();

		if(lightingType==LIGHTING_TYPE_SH_UNSHADOWED)
		{
			for(int j=0; j<numVertices; ++j)
			{
				Vertex & currentVertex=currentObject->vertices[j];

				double brightness=0.0;

				for(int k=0; k < light.numFunctions; ++k)
					//brightness += light.rotatedLightCoeffs[k]*currentVertex.unshadowedCoeffs[k];
					brightness += light.coeffs[k]*currentVertex.unshadowedCoeffs[k];
				
				currentVertex.litColor=(float)brightness*currentVertex.diffuseMaterial;
			}
		}

		if(lightingType==LIGHTING_TYPE_SH_SHADOWED)
		{
			for(int j=0; j<numVertices; ++j)
			{
				Vertex & currentVertex=currentObject->vertices[j];

				double brightness=0.0;

				for(int k = 0; k < light.numFunctions; ++k)
					//brightness += light.rotatedLightCoeffs[k]*currentVertex.shadowedCoeffs[k];
					brightness += light.coeffs[k]*currentVertex.shadowedCoeffs[k];
				
				currentVertex.litColor=(float)brightness*currentVertex.diffuseMaterial;
			}
		}
	}
}

void PRTRenderer::renderSceneWithLight(Scene& scene, Light& light)
{
	while (!glfwWindowShouldClose(window)) {

		if (bAnim) changeMatrics();
		precomputeColor(scene, light);
		scene.bindBuffer();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		//glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, scene.VB);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)0);	// position
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)(sizeof(glm::vec3)+sizeof(glm::vec3)));// color
		//glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)(sizeof(glm::vec3)+sizeof(glm::vec2)));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, scene.IB);

		glDrawElements(GL_TRIANGLES, scene.numIndices, GL_UNSIGNED_INT, 0);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		//glDisableVertexAttribArray(2);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void PRTRenderer::render2(Scene& scene)
{
	while (!glfwWindowShouldClose(window)) {
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);

		if (bAnim) changeMatrics();

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		//glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, scene.VB);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)0);	// position
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)(sizeof(glm::vec3)+sizeof(glm::vec3)));// color
		//glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)(sizeof(glm::vec3)+sizeof(glm::vec2)));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, scene.IB);

		glDrawElements(GL_TRIANGLES, scene.numIndices, GL_UNSIGNED_INT, 0);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		//glDisableVertexAttribArray(2);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}


// glfw call-back function
//========================================================================
// Print errors
//========================================================================
void PRTRenderer::error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

//========================================================================
// Handle key strokes
//========================================================================
void PRTRenderer::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	switch (key) {
	case GLFW_KEY_ESCAPE:
		if (action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);
		break;
	case GLFW_KEY_SPACE:
		if (action == GLFW_PRESS) {
			cout << "set animation\n" << endl;
			bAnim = !bAnim;
		}
		break;
	case GLFW_KEY_1:
		//lightingType = LIGHTING_TYPE_GL;
		break;

	case GLFW_KEY_2:
		lightingType = LIGHTING_TYPE_SH_UNSHADOWED;
		break;
	case GLFW_KEY_3:
		lightingType = LIGHTING_TYPE_SH_SHADOWED;
	}
}

//========================================================================
// Callback function for mouse button events
//========================================================================
void PRTRenderer::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button != GLFW_MOUSE_BUTTON_LEFT)
        return;

    if (action == GLFW_PRESS) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        locked = true;
    }
    else {
        locked = false;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

//========================================================================
// Callback function for cursor motion events
//========================================================================
void PRTRenderer::cursor_position_callback(GLFWwindow* window, double x, double y)
{
    if (locked)
    {
        alpha += (GLfloat) (x - cursorX) / 10.f;
        beta += (GLfloat) (y - cursorY) / 10.f;
    }

    cursorX = (int) x;
    cursorY = (int) y;
}

//========================================================================
// Callback function for scroll events
//========================================================================
void PRTRenderer::scroll_callback(GLFWwindow* window, double x, double y)
{
    zoom += (float) y / 4.f;
    if (zoom < 0)
        zoom = 0;
}


//========================================================================
// Callback function for framebuffer resize events
//========================================================================
void PRTRenderer::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    float ratio = 1.f;

    if (height > 0)
        ratio = (float) width / (float) height;

    // Setup viewport
    glViewport(0, 0, width, height);

    // Change to the projection matrix and set our viewing volume
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, ratio, 0.1f, 1000.0f);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
}
