 #include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "Shaders.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb_image.h"

#define WINDOW_HEIGHT 600
#define WINDOW_WIDTH 800


#define SKYBOX_PATH_DOWN "C:\\Users\\Kajkacz\\Desktop\\Projekty\\ProjektGKOM\\Resources\\Resources\\Skybox\\skyrender0006.bmp"
#define SKYBOX_PATH_TOP "C:\\Users\\Kajkacz\\Desktop\\Projekty\\ProjektGKOM\\Resources\\Resources\\Skybox\\skyrender0003.bmp"
#define SKYBOX_PATH_BACK "C:\\Users\\Kajkacz\\Desktop\\Projekty\\ProjektGKOM\\Resources\\Resources\\Skybox\\skyrender0005.bmp"
#define SKYBOX_PATH_FRONT "C:\\Users\\Kajkacz\\Desktop\\Projekty\\ProjektGKOM\\Resources\\Resources\\Skybox\\skyrender0002.bmp"
#define SKYBOX_PATH_RIGHT "C:\\Users\\Kajkacz\\Desktop\\Projekty\\ProjektGKOM\\Resources\\Resources\\Skybox\\skyrender0001.bmp"
#define SKYBOX_PATH_LEFT  "C:\\Users\\Kajkacz\\Desktop\\Projekty\\ProjektGKOM\\Resources\\Resources\\Skybox\\skyrender0004.bmp"
#define DECK_TEXTURE  "C:\\Users\\Kajkacz\\Desktop\\Projekty\\ProjektGKOM\\Resources\\Resources\\Deck.bmp"

//Camera details
glm::vec3 cameraPos = glm::vec3(0.0f, 2.0f, 2.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, -2.0f, -2.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
//Animation
float rudderAngle = 0.0f;
//Light
glm::vec3 lightColor;
float lightStrenght = 0.1;
//Skyboxs
unsigned int skybox[6];
enum { SKY_LEFT = 0, SKY_BACK, SKY_RIGHT, SKY_FRONT, SKY_TOP, SKY_BOTTOM };
bool loadSkyboxSide(unsigned int texture, GLenum targetSide, const char*fileName);
void makeSkyBox(const char* front, const char* back, const char* top, const char* bottom, const char* left, const char* right, unsigned int * cubeTexture);

void framebuffer_size_callback(GLFWwindow* window, int width, int height); //Resizeing moving window
void processInput(GLFWwindow *window);
void renderThings(GLFWwindow *window);


int main()
{
	if (!glfwInit()) {
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);	//Setting range of servicable glfw versions and other init options
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "ProjektGKOMOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "failed to create glfw window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	Shaders ourShader("Shader.vect", "Shader.frag");

	float width, height, lenght;
	height = lenght = width = 0.2f;
	

	float boatVertices[] = {
		// positions          // colors         
		-width,		0.0f,		-lenght,	0.0f, 0.0f, 0.6f,
		width,		0.0f,		-lenght,	0.0f, 0.0f, 0.6f,
		width,		0.0f,		lenght,		0.0f, 0.0f, 0.6f,
		-width,		0.0f,		lenght,		0.0f, 0.0f, 0.6f,
		0.0f,		0.0f,		2 * lenght, 0.0f, 0.0f, 0.6f,
		0.0f,		-height,	-lenght,	0.0f, 0.0f, 0.6f,
		0.0f,		-height,	lenght,		0.0f, 0.0f, 0.6f
	};

	unsigned int boatIndices[] = { //Poprawiæ, do przesuniêcia o 1 do ty³u ! ! !
		0,5,3,
		5,6,3,
		1,2,5,
		5,6,2,
		0,1,5,
		2,3,4,
		2,6,4,
		3,4,6,
		2,3,4
	};

	float boatDeckVertices[] = {
		// positions          // colors         
		-width,		0.0f,		-lenght,	0.0f, 0.0f, 1.0f,		0.0f, 0.0f,
		width,		0.0f,		-lenght,	0.0f, 0.0f, 1.0f,		0.0f, 1.0f,	
		width,		0.0f,		lenght,		0.0f, 0.0f, 1.0f,		1.0f, 1.0f,
		-width,		0.0f,		lenght,		0.0f, 0.0f, 1.0f,		1.0f, 0.0f	
	};

	unsigned int boatDeckIndices[] = { 
		0,1,3,
		1,2,3
	};

	float ruderWidth;
	ruderWidth = 0.02*width;
	
	float rudderVertices[] = {							
		ruderWidth,		0.05*height,	-0.8*lenght,	0.5f, 0.0f, 0.0f,
		ruderWidth,		0.02*height,	-0.8*lenght,	0.5f, 0.0f, 0.0f,
		ruderWidth,		0.02*height,	-1.02*lenght,	0.5f, 0.0f, 0.0f,
		ruderWidth,		-1.2*height,	-1.02*lenght,	0.5f, 0.0f, 0.0f,
		ruderWidth,		-1.4*height,	-1.3*lenght,	0.5f, 0.0f, 0.0f,
		ruderWidth,		0.05*height,	-1.3*lenght,	0.5f, 0.0f, 0.0f,
															
		-ruderWidth,	0.05*height,	-0.8*lenght,	0.5f, 0.0f, 0.0f,
		-ruderWidth,	0.02*height,	-0.8*lenght,	0.5f, 0.0f, 0.0f,
		-ruderWidth,	0.02*height,	-1.02*lenght,	0.5f, 0.0f, 0.0f,
		-ruderWidth,	-1.2*height,	-1.02*lenght,	0.5f, 0.0f, 0.0f,
		-ruderWidth,	-1.4*height,	-1.3*lenght,	0.5f, 0.0f, 0.0f,
		-ruderWidth,	0.05*height,	-1.3*lenght,	0.5f, 0.0f, 0.0f
	};

	unsigned int rudderIndices[] = {
		0,1,2,
		0,2,5,
		5,2,4,
		2,3,4,

		6,7,8,
		6,8,11,
		8,11,4,
		8,9,10,

		0,5,11,
		6,0,11,

		3,9,4,
		10,9,3,

		0,1,6,
		0,1,7,

		1,7,2,
		1,7,8,

		2,8,9,
		2,8,3,

		4,10,5,
		4,10,11
	};

	float postWidth = 0.02f;
	float postHeight = 3 * height / 4;
	float postPosition = -0.4*lenght;
	float postVertices[] =
	{
	postWidth,	0.0f,		postPosition			,	0.1f,0.0f,0.0f,
	-postWidth,	0.0f,		postPosition			,	0.1f,0.0f,0.0f,
	-postWidth,	0.0f,		postPosition - postWidth,	0.1f,0.0f,0.0f,
	postWidth,	0.0f,		postPosition - postWidth,	0.1f,0.0f,0.0f,
 
	postWidth,	postHeight,	postPosition,				0.1f,0.0f,0.0f,
	-postWidth,	postHeight,	postPosition,				0.1f,0.0f,0.0f,
	-postWidth,	postHeight,	postPosition - postWidth, 	0.1f,0.0f,0.0f,
	postWidth,	postHeight,	postPosition - postWidth, 	0.1f,0.0f,0.0f
	};

	unsigned int postIndices[] =
	{
		0,3,4,
		0,3,7,

		0,4,5,
		1,4,5,
		
		2,6,7,
		2,6,3,

		2,6,1,
		2,6,5,
		
		7,3,2,
		7,3,6,
		
		4,5,6,
		4,5,7
	};

	float wheelRadius = 0.05f;
	float wheelThickness = 0.005f;
	float wheelWidth = 0.005;
	postHeight -= 0.01f;
	float wheelVertices[] = {

		wheelWidth,						postHeight + wheelWidth ,					postPosition -	postWidth,									0.1f,0.0f,0.0f,
		-wheelWidth,					postHeight + wheelWidth	,					postPosition - postWidth,									0.1f,0.0f,0.0f,
		-wheelWidth,					postHeight - wheelWidth,					postPosition - postWidth,									0.1f,0.0f,0.0f,
		+wheelWidth,					postHeight - wheelWidth,					postPosition - postWidth,									0.1f,0.0f,0.0f,
																									 
		wheelWidth,						postHeight + wheelWidth + wheelRadius ,		postPosition -	postWidth,									0.1f,0.0f,0.0f,
		-wheelWidth,					postHeight + wheelWidth + wheelRadius ,		postPosition - postWidth,									0.1f,0.0f,0.0f,
	
		-wheelWidth - wheelRadius ,		postHeight + wheelWidth,					postPosition - postWidth,									0.1f,0.0f,0.0f,
		-wheelWidth - wheelRadius ,		postHeight - wheelWidth,					postPosition -	postWidth,									0.1f,0.0f,0.0f,

		-wheelWidth,					postHeight - wheelWidth - wheelRadius ,		postPosition - postWidth,									0.1f,0.0f,0.0f,
		wheelWidth,						postHeight - wheelWidth - wheelRadius ,		postPosition - postWidth,									0.1f,0.0f,0.0f,
																									 
		wheelWidth + wheelRadius ,		postHeight - wheelWidth,					postPosition -	postWidth,									0.1f,0.0f,0.0f,
		wheelWidth + wheelRadius ,		postHeight + wheelWidth,					postPosition - postWidth,									0.1f,0.0f,0.0f,
	

		wheelWidth,						postHeight + wheelWidth ,					-wheelThickness + postPosition - postWidth,					0.1f,0.0f,0.0f,
		-wheelWidth,					postHeight + wheelWidth	,					-wheelThickness + postPosition - postWidth,					0.1f,0.0f,0.0f,
		-wheelWidth,					postHeight - wheelWidth,					-wheelThickness + postPosition - postWidth,					0.1f,0.0f,0.0f,
		+wheelWidth,					postHeight - wheelWidth,					-wheelThickness + postPosition - postWidth,					0.1f,0.0f,0.0f,

		wheelWidth,						postHeight + wheelWidth + wheelRadius ,		-wheelThickness + postPosition - postWidth,					0.1f,0.0f,0.0f,
		-wheelWidth,					postHeight + wheelWidth + wheelRadius ,		-wheelThickness + postPosition - postWidth,					0.1f,0.0f,0.0f,
	
		-wheelWidth - wheelRadius ,		postHeight + wheelWidth,					-wheelThickness + postPosition - postWidth,					0.1f,0.0f,0.0f,
		-wheelWidth - wheelRadius ,		postHeight - wheelWidth,					-wheelThickness + postPosition - postWidth,					0.1f,0.0f,0.0f,
	
		-wheelWidth,					postHeight - wheelWidth - wheelRadius ,		-wheelThickness + postPosition - postWidth,					0.1f,0.0f,0.0f,
		wheelWidth,						postHeight - wheelWidth - wheelRadius ,		-wheelThickness + postPosition - postWidth,					0.1f,0.0f,0.0f,

		wheelWidth + wheelRadius ,		postHeight - wheelWidth,					-wheelThickness + postPosition - postWidth,					0.1f,0.0f,0.0f,
		wheelWidth + wheelRadius ,		postHeight + wheelWidth,					-wheelThickness + postPosition - postWidth,					0.1f,0.0f,0.0f
		
	};																																			
	unsigned int wheelIndices[] =
	{
		1,0,3,
		1,2,3,
		5,4,0,
		5,1,0,
		0,3,10,
		0,11,10,
		2,8,9,
		2,3,9,
		1,2,6,
		2,6,7,

		17,16,12,
		17,13,12,
		13,12,15,
		13,14,15,
		12,22,15,
		12,22,23,
		14,15,21,
		14,20,21,
		18,13,14,
		18,19,14,

		4,5,17,
		4,5,16,
		1,5,17,
		1,5,13,
		1,6,18,
		1,6,19,
		6,7,18,
		6,7,19,
		7,2,14,
		7,2,19,
		2,8,14,
		2,8,20,
		8,9,20,
		8,9,21,
		9,3,15,
		9,3,21,
		3,10,15,
		3,10,22,
		10,11,22,
		10,11,23,
		11,0,12,
		11,0,23,
		0,4,16,
		0,4,12
	
	};


	float mastWidth = 0.02f;
	float mastHeight = 3 * height;
	float mastPosition = 0.8*lenght;
	float mastVertices[] =
	{
		mastWidth,	0.0f,		mastPosition			,	0.1f,0.0f,0.0f,
		-mastWidth,	0.0f,		mastPosition			,	0.1f,0.0f,0.0f,
		-mastWidth,	0.0f,		mastPosition - mastWidth,	0.1f,0.0f,0.0f,
		mastWidth,	0.0f,		mastPosition - mastWidth,	0.1f,0.0f,0.0f,

		mastWidth,	mastHeight,	mastPosition,				0.1f,0.0f,0.0f,
		-mastWidth,	mastHeight,	mastPosition,				0.1f,0.0f,0.0f,
		-mastWidth,	mastHeight,	mastPosition - mastWidth, 	0.1f,0.0f,0.0f,
		mastWidth,	mastHeight,	mastPosition - mastWidth, 	0.1f,0.0f,0.0f
	};

	unsigned int mastIndices[] =
	{
		0,3,4,
		0,3,7,

		0,4,5,
		1,4,5,

		2,6,1,
		2,6,5,

		2,6,7,
		2,6,3,

		7,3,2,
		7,3,6,

		4,5,6,
		4,5,7
	};
	
	float boomWidth = 0.02f;
	float boomLenght= 2 * height;
	float boomVertices[] =
	{
		mastWidth,				height + boomWidth	,		mastPosition			,	0.1f,0.0f,0.0f,
		mastWidth,				height				,		mastPosition			,	0.1f,0.0f,0.0f,
		mastWidth + boomLenght,	height				,		mastPosition			,	0.1f,0.0f,0.0f,
		mastWidth + boomLenght,	height + boomWidth	,		mastPosition			,	0.1f,0.0f,0.0f,
																						  
		mastWidth,				height + boomWidth	,		mastPosition + boomWidth,	0.1f,0.0f,0.0f,
		mastWidth,				height				,		mastPosition + boomWidth,	0.1f,0.0f,0.0f,
		mastWidth + boomLenght,	height				,		mastPosition + boomWidth,	0.1f,0.0f,0.0f,
		mastWidth + boomLenght,	height + boomWidth	,		mastPosition + boomWidth,	0.1f,0.0f,0.0f,

		mastWidth,				height + boomWidth	,		mastPosition			,	1.0f,1.0f,1.0f,
		mastWidth + boomLenght,	height + boomWidth	,		mastPosition			,	1.0f,1.0f,1.0f,
		mastWidth,				mastHeight			,		mastPosition			,	1.0f,1.0f,1.0f,

	};

	unsigned int boomIndices[] =
	{
		0,1,2,
		0,1,3,

		4,5,6,
		4,5,7,

		0,4,7,
		0,7,3,

		7,3,2,
		7,2,6,

		6,2,1,
		6,2,5,

		0,1,4,
		0,1,5,

		7,6,4,

		8,9,10
	};

	float farAway = 100.0f;
	float seaLevel = -0.5*height;
	float seaVertices[] = {
		farAway, seaLevel , farAway, 0.0f, 0.5f,0.75f,
		farAway, seaLevel , -farAway, 0.0f, 0.5f,0.75f,
		-farAway, seaLevel , -farAway, 0.0f, 0.5f,0.75f,
		-farAway, seaLevel , farAway, 0.0f, 0.5f,0.75f
	};

	unsigned int seaIndices[] = {
		0,1,2,
		0,2,3
	};

	float skyBoxDistance = 50.0f;
	float skybox[] = {
		-skyBoxDistance,  skyBoxDistance, -skyBoxDistance,
		-skyBoxDistance, -skyBoxDistance, -skyBoxDistance,
		skyBoxDistance, -skyBoxDistance, -skyBoxDistance,
		skyBoxDistance, -skyBoxDistance, -skyBoxDistance,
		skyBoxDistance,  skyBoxDistance, -skyBoxDistance,
		-skyBoxDistance,  skyBoxDistance, -skyBoxDistance,

		-skyBoxDistance, -skyBoxDistance,  skyBoxDistance,
		-skyBoxDistance, -skyBoxDistance, -skyBoxDistance,
		-skyBoxDistance,  skyBoxDistance, -skyBoxDistance,
		-skyBoxDistance,  skyBoxDistance, -skyBoxDistance,
		-skyBoxDistance,  skyBoxDistance,  skyBoxDistance,
		-skyBoxDistance, -skyBoxDistance,  skyBoxDistance,

		skyBoxDistance, -skyBoxDistance, -skyBoxDistance,
		skyBoxDistance, -skyBoxDistance,  skyBoxDistance,
		skyBoxDistance,  skyBoxDistance,  skyBoxDistance,
		skyBoxDistance,  skyBoxDistance,  skyBoxDistance,
		skyBoxDistance,  skyBoxDistance, -skyBoxDistance,
		skyBoxDistance, -skyBoxDistance, -skyBoxDistance,

		-skyBoxDistance, -skyBoxDistance,  skyBoxDistance,
		-skyBoxDistance,  skyBoxDistance,  skyBoxDistance,
		skyBoxDistance,  skyBoxDistance,  skyBoxDistance,
		skyBoxDistance,  skyBoxDistance,  skyBoxDistance,
		skyBoxDistance, -skyBoxDistance,  skyBoxDistance,
		-skyBoxDistance, -skyBoxDistance,  skyBoxDistance,

		-skyBoxDistance,  skyBoxDistance, -skyBoxDistance,
		skyBoxDistance,  skyBoxDistance, -skyBoxDistance,
		skyBoxDistance,  skyBoxDistance,  skyBoxDistance,
		skyBoxDistance,  skyBoxDistance,  skyBoxDistance,
		-skyBoxDistance,  skyBoxDistance,  skyBoxDistance,
		-skyBoxDistance,  skyBoxDistance, -skyBoxDistance,

		-skyBoxDistance, -skyBoxDistance, -skyBoxDistance,
		-skyBoxDistance, -skyBoxDistance,  skyBoxDistance,
		skyBoxDistance, -skyBoxDistance, -skyBoxDistance,
		skyBoxDistance, -skyBoxDistance, -skyBoxDistance,
		-skyBoxDistance, -skyBoxDistance,  skyBoxDistance,
		skyBoxDistance, -skyBoxDistance,  skyBoxDistance
	};
	
	unsigned int VBO, VAO, EBO, VBOBD, VAOBD, EBOBD, VBOR, VAOR, EBOR, VBOP, VAOP, EBOP, VBOW, VAOW, EBOW, VBOM, VAOM, EBOM, VBOB, VAOB, EBOB, VBOS, VAOS, EBOS , VBOSB, VAOSB;

	//Boat

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(boatVertices), boatVertices, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(boatIndices), boatIndices, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//BoatDeck

	glGenVertexArrays(1, &VAOBD);
	glGenBuffers(1, &VBOBD);
	glGenBuffers(1, &EBOBD);
	glBindVertexArray(VAOBD);

	glBindBuffer(GL_ARRAY_BUFFER, VBOBD);
	glBufferData(GL_ARRAY_BUFFER, sizeof(boatDeckVertices), boatDeckVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOBD);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(boatDeckIndices), boatDeckIndices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
	//Sea

	glGenVertexArrays(1, &VAOS);
	glGenBuffers(1, &VBOS);
	glGenBuffers(1, &EBOS);
	glBindVertexArray(VAOS);

	glBindBuffer(GL_ARRAY_BUFFER, VBOS);
	glBufferData(GL_ARRAY_BUFFER, sizeof(seaVertices), seaVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOS);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(seaIndices), seaIndices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	//Boom

	glGenVertexArrays(1, &VAOB);
	glGenBuffers(1, &VBOB);
	glGenBuffers(1, &EBOB);
	glBindVertexArray(VAOB);

	glBindBuffer(GL_ARRAY_BUFFER, VBOB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(boomVertices), boomVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(boomIndices), boomIndices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//Rudder	

	glGenVertexArrays(1, &VAOR);
	glGenBuffers(1, &VBOR);
	glGenBuffers(1, &EBOR);

	glBindVertexArray(VAOR);

	glBindBuffer(GL_ARRAY_BUFFER, VBOR);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rudderVertices), rudderVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOR);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rudderIndices), rudderIndices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//Post

	glGenVertexArrays(1, &VAOP);
	glGenBuffers(1, &VBOP);
	glGenBuffers(1, &EBOP);
	glBindVertexArray(VAOP);

	glBindBuffer(GL_ARRAY_BUFFER, VBOP);
	glBufferData(GL_ARRAY_BUFFER, sizeof(postVertices), postVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOP);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(postIndices), postIndices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//Mast

	glGenVertexArrays(1, &VAOM);
	glGenBuffers(1, &VBOM);
	glGenBuffers(1, &EBOM);
	glBindVertexArray(VAOM);

	glBindBuffer(GL_ARRAY_BUFFER, VBOM);
	glBufferData(GL_ARRAY_BUFFER, sizeof(mastVertices), mastVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOM);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(mastIndices), mastIndices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	//Wheel

	glGenVertexArrays(1, &VAOW);
	glGenBuffers(1, &VBOW);
	glGenBuffers(1, &EBOW);
	glBindVertexArray(VAOW);

	glBindBuffer(GL_ARRAY_BUFFER, VBOW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(wheelVertices), wheelVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(wheelIndices), wheelIndices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glEnable(GL_DEPTH_TEST);

	glGenVertexArrays(1, &VAOSB);
	glGenBuffers(1, &VBOSB);
	glBindBuffer(GL_ARRAY_BUFFER, VBOSB);
	glBufferData(GL_ARRAY_BUFFER, 3 * 36 * sizeof(float), skybox,GL_STATIC_DRAW);

	glBindVertexArray(VAOSB);
	
	glVertexAttribPointer(0,3, GL_FLOAT, GL_FALSE,0,NULL);
	glEnableVertexAttribArray(0);

	Shaders skyBoxShader("SkyBoxShader.vect", "SkyBoxShader.frag");
	Shaders TXShader("ShaderTX.vect", "ShaderTX.frag");


	unsigned int skyBoxTexture;
	makeSkyBox(SKYBOX_PATH_FRONT, SKYBOX_PATH_BACK, SKYBOX_PATH_TOP, SKYBOX_PATH_DOWN, SKYBOX_PATH_LEFT, SKYBOX_PATH_RIGHT, &skyBoxTexture);
	unsigned int deckTexture;
	
	glGenTextures(1, &deckTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, deckTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int TWidth, THeight, nrChannels;
	unsigned char *data = stbi_load(DECK_TEXTURE, &TWidth, &THeight, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TWidth, THeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);


	
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);

	ourShader.use();
	lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	lightStrenght = 1.0f;
	ourShader.setMat4("projection", projection);
	//ourShader.setFloat("ambientStr", lightStrenght);
	//ourShader.setVec3("lightColor", lightColor);

	skyBoxShader.use();
	skyBoxShader.setMat4("Projection", projection);
	
	TXShader.use();
	TXShader.setMat4("projection", projection);
	TXShader.setInt("ourTexture", deckTexture);

	glm::vec3 previousCameraPos = cameraPos;
	glm::vec3 previousCameraFront = cameraFront;

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

	
		//Camera
		glm::mat4 view,transW,transR, transB;
		//unsigned int transLoc;


		view = glm::lookAt(cameraPos, cameraFront + cameraPos, cameraUp);
				
		glClearColor(0.5f, 0.5f, 0.5f, 0.5f);
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);

		//RenderSkybox
		glDepthMask(GL_FALSE);
		skyBoxShader.use();
		skyBoxShader.setMat4("View", view);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skyBoxTexture);
		glBindVertexArray(VAOSB);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDepthMask(GL_TRUE);

		//Render boat deck
		TXShader.use(); 
		TXShader.setMat4("view", view);
		
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, deckTexture);
		glBindVertexArray(VAOBD);
		glDrawElements(GL_TRIANGLES, _countof(boatDeckVertices), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		ourShader.use();
		ourShader.setMat4("view", view);
		ourShader.setMat4("transform", transW);
		//Render boat
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, _countof(boatVertices), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		//Render sea
		glBindVertexArray(VAOS);
		glDrawElements(GL_TRIANGLES, _countof(seaVertices), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		//Render post
		glBindVertexArray(VAOP);
		glDrawElements(GL_TRIANGLES, _countof(postVertices), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		//Render mast
		glBindVertexArray(VAOM);
		glDrawElements(GL_TRIANGLES, _countof(mastVertices), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		//Render boom and sail
			//Animate boom
		
		transB = glm::translate(transB, glm::vec3(0, 0, mastPosition ));
		transB = glm::rotate(transB, glm::radians( rudderAngle + 45.0f + (rand()%100)/200), glm::vec3(0, 1, 0));
		transB = glm::translate(transB, glm::vec3(0, 0, -mastPosition ));
		ourShader.setMat4("transform", transB);

		glBindVertexArray(VAOB);
		glDrawElements(GL_TRIANGLES, _countof(boomVertices), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		//Render wheel
			//Animate wheel
		transW = glm::translate(transW, glm::vec3(0, postHeight, -postPosition + postWidth));
		transW = glm::rotate(transW, glm::radians(3*rudderAngle), glm::vec3(0, 0, 1));
		transW = glm::translate(transW, glm::vec3(0, -postHeight, postPosition - postWidth));
		ourShader.setMat4("transform", transW);
		glBindVertexArray(VAOW);
		glDrawElements(GL_TRIANGLES, _countof(wheelVertices), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		//Render rudder
			//Animate rudder
		transR = glm::translate(transR, glm::vec3(0, 0, -lenght));
		transR = glm::rotate(transR,glm::radians(rudderAngle),glm::vec3(0,1,0));
		transR = glm::translate(transR, glm::vec3(0, 0, lenght));
			
		ourShader.setMat4("transform", transR);
		glBindVertexArray(VAOR);
		glDrawElements(GL_TRIANGLES, _countof(rudderVertices), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glfwSwapBuffers(window);

		glfwPollEvents();

		if (rudderAngle >= 30.0f)
			rudderAngle = 30.0f;

		if (rudderAngle <= -30.0f)
			rudderAngle = -30.0f;

	 }

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	//Camera
	float cameraSpeed = 0.05f;
	float cameraTurnSpeed = 0.02f;
	float rudderStirde = 1.0f;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -=glm::normalize(glm::cross(cameraFront,cameraUp))* cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		cameraPos += cameraUp * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		cameraPos -= cameraUp * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		cameraFront -= glm::normalize(glm::cross(cameraFront, cameraUp))* cameraTurnSpeed;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		cameraFront += glm::normalize(glm::cross(cameraFront,cameraUp)) * cameraTurnSpeed;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		cameraFront += cameraUp * cameraTurnSpeed;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		cameraFront -= cameraUp * cameraTurnSpeed;
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		rudderAngle += rudderStirde;
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		rudderAngle -= rudderStirde;
}


void makeSkyBox(const char* front , const char* back, const char* top, const char* bottom, const char* left, const char* right, unsigned int * cubeTexture)
{
	//Handle cube textures
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, cubeTexture);

	//Load all the textures into the cube
	loadSkyboxSide(*cubeTexture, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, front);
	loadSkyboxSide(*cubeTexture, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, back);
	loadSkyboxSide(*cubeTexture, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, top);
	loadSkyboxSide(*cubeTexture, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, bottom);
	loadSkyboxSide(*cubeTexture, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, left);
	loadSkyboxSide(*cubeTexture, GL_TEXTURE_CUBE_MAP_POSITIVE_X, right);

	// format cube map texture
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

bool loadSkyboxSide(unsigned int texture, GLenum targetSide, const char*fileName)
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

	int x, y, n;
	int forceChannels = 4;
	unsigned char* imageData = stbi_load(fileName, &x, &y, &n, forceChannels);

	if (!imageData)
	{
		std::cout << "\nFailed to load skybox texture\n" << std::endl;
		return false;
	}
	if ((x&(x - 1)) != 0 || (y&(y - 1)) != 0)
	{
		std::cout << "Warning! Image " << fileName << " is not 2 - dimensional! \n" << std::endl;
	}

	glTexImage2D(targetSide, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

		return true;
}
