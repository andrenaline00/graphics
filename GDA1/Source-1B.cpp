//********************************
//
//ΑΜ:5309                     Όνομα:Andreana Nikou
//ΑΜ:5379                     Όνομα:Theodora Ferentinou

//*********************************

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;
using namespace std;

/// <summary>
/// //////////////////////////////////////////////
/// </summary>
glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;


glm::mat4 getViewMatrix() {
	return ViewMatrix;
}
glm::mat4 getProjectionMatrix() {
	return ProjectionMatrix;
}

////////////////////////////////////////////// <summary>
/// Add camera function here
/// </summary>

void camera_function()
{

}

/////////////////////////////////////////////////

GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path) {

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;


	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const* VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}



	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const* FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}



	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}


	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}
///////////////////////////////////////////////////



int main(void)
{
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(850, 850, u8"Εργασία 1Β - 2025 - Σκηνικό Παιχνιδιού", NULL, NULL);


	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// background color - dark gray
	glClearColor(34.0f / 255.0f, 34.0f / 255.0f, 34.0f / 255.0f, 0.0f);

	const size_t CUBE_COLOR_SIZE = 36 * 4; // 36 κορυφές * 4 components (RGBA)
	GLfloat cubecolor[CUBE_COLOR_SIZE];

	glEnable(GL_DEPTH_TEST);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders

	GLuint programID = LoadShaders("P1BVertexShader.vertexshader", "P1BFragmentShader.fragmentshader");

	GLuint MatrixID = glGetUniformLocation(programID, "MVP");

	glm::mat4 Projection = glm::perspective(glm::radians(60.0f), 4.0f / 4.0f, 0.1f, 100.0f);

	// Camera matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(0.0f, -5.0f, 20.0f), // Camera in World Space
		glm::vec3(0.0f, 0.0f, 0.0f), // and looks at the origin
		glm::vec3(0.0f, 1.0f, 0.0f));  // Head is up 
	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 Model = glm::mat4(1.0f);
	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 MVP = Projection * View * Model;

	//added these
	glm::vec3 cameraPos = glm::vec3(0.0f, -5.0f, 20.0f);
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	//camera
	float rotX = 0.0f; //rotation around x axis
	float rotY = 0.0f; //rotation around y axis
	float distance = 20.0f; //zoom in/out

	//charA variables
	const float STEP = 1.5f;
	float charA_x = 0.0f; //position on x axis
	//float charA_y = -5.0f; //position on y axis

	static double lastTime = glfwGetTime();
	double currentTime;
	float deltaTime;


	GLfloat len = 5.0f, wid = 2.5f, heig = 2.5f;


	static const GLfloat cube[] =
	{
		//front-up-left triagle
		-9.0f,10.0f, 0.0f, //K
		-7.0f,10.0f, 0.0f, //front-up-right
		-9.0f, 8.0f, 0.0f, //front-down-left

		//front-down-right triangle
		-7.0f,10.0f, 0.0f, //front-up-right
		-7.0f, 8.0f, 0.0f, //front-down-right
		-9.0f, 8.0f, 0.0f, //front-down-left

		//right-up-left triangle
		-7.0f,10.0f, 0.0f,
		-7.0f, 8.0f, 0.0f,
		-7.0f,10.0f,-2.0f,

		//right-down-right triangle
		-7.0f, 8.0f, 0.0f,
		-7.0f,10.0f,-2.0f,
		-7.0f, 8.0f,-2.0f,

		//behind-up-right triangle
		-7.0f,10.0f,-2.0f,
		-7.0f, 8.0f,-2.0f,
		-9.0f,10.0f,-2.0f,

		//behind-down-left triangle
		-7.0f, 8.0f,-2.0f,
		-9.0f,10.0f,-2.0f,
		-9.0f, 8.0f,-2.0f,

		//left-up-right triangle
		-9.0f,10.0f, 0.0f, //K
		-9.0f,10.0f,-2.0f,
		-9.0f, 8.0f,-2.0f,

		//left-down-left triangle
		-9.0f,10.0f, 0.0f, //K
		-9.0f, 8.0f,-2.0f,
		-9.0f, 8.0f, 0.0f,

		//up-up-left triangle
		-9.0f,10.0f, 0.0f, //K
		-9.0f,10.0f,-2.0f,
		-7.0f,10.0f,-2.0f,

		//up-down-right triangle
		-9.0f,10.0f, 0.0f, //K
		-7.0f,10.0f, 0.0f,
		-7.0f,10.0f,-2.0f,

		//down-up-left triangle
		-9.0f, 8.0f, 0.0f,
		-9.0f, 8.0f,-2.0f,
		-7.0f, 8.0f,-2.0f,

		//down-down-right triangle
		-7.0f, 8.0f, 0.0f,
		-9.0f, 8.0f, 0.0f,
		-7.0f, 8.0f,-2.0f,
	};


	static const GLfloat charA[] =
	{
		//4 triangles apo v9 pros 4 akres panw vasis
		 0.0f,-7.75f,-1.0f, //v9
		-1.5f,-8.5f ,-2.0f, //v8
		-1.5f,-8.5f , 0.0f, //v4

		 0.0f,-7.75f,-1.0f, //v9
		-1.5f,-8.5f , 0.0f, //v4
		 1.5f,-8.5f, 0.0f,  //v3

		 0.0f,-7.75f,-1.0f, //v9
		 1.5f,-8.5f, 0.0f,  //v3
		 1.5f,-8.5f ,-2.0f, //v7

		 0.0f,-7.75f,-1.0f, //v9
		 1.5f,-8.5f ,-2.0f, //v7
		-1.5f,-8.5f ,-2.0f, //v8

		-1.5f,-8.5f ,-2.0f, //v8
		-1.5f,-8.5f , 0.0f, //v4
		-1.5f,-10.0f, 0.0f, //v1

		-1.5f,-8.5f ,-2.0f, //v8
		-1.5f,-10.0f, 0.0f, //v1
		-1.5f,-10.0f,-2.0f, //v5

		-1.5f,-8.5f ,-2.0f, //v8
		-1.5f,-10.0f,-2.0f, //v5
		 1.5f,-10.0f,-2.0f, //v6

		-1.5f,-8.5f ,-2.0f, //v8
		 1.5f,-8.5f ,-2.0f, //v7
		 1.5f,-10.0f,-2.0f, //v6

		 1.5f,-8.5f ,-2.0f, //v7
		 1.5f,-10.0f,-2.0f, //v6
		 1.5f,-10.0f, 0.0f, //v2

		 1.5f,-8.5f ,-2.0f, //v7
		 1.5f,-10.0f, 0.0f, //v2
		 1.5f,-8.5f, 0.0f,  //v3

		-1.5f,-8.5f , 0.0f, //v4
		 1.5f,-10.0f, 0.0f, //v2
		 1.5f,-8.5f, 0.0f,  //v3

		-1.5f,-8.5f , 0.0f, //v4
		 1.5f,-10.0f, 0.0f, //v2
		-1.5f,-10.0f, 0.0f, //v1
	};




	GLfloat a = 0.4f;
	static const GLfloat charColor[] = {
		1.0f, 0.0f, 0.0f, a, //red  //v9 
		1.0f, 0.0f, 0.0f, a,  //v8 
		1.0f, 0.0f, 0.0f, a,  //v4

		1.0f, 1.0f, 0.0f, a,  //yellow //v9
		1.0f, 1.0f, 0.0f, a,   //v4
		1.0f, 1.0f, 0.0f, a,   //v3

		0.0f, 1.0f, 0.0f, a,  //green//v9
		0.0f, 1.0f, 0.0f, a,   //v3
		0.0f, 1.0f, 0.0f, a,  //v7

		1.0f, 0.0f, 1.0f, a,  //ppurple //v9
		1.0f, 0.0f, 1.0f, a,   //v7
		1.0f, 0.0f, 1.0f, a,  //v8

		0.0f, 1.0f, 1.0f, a,  //cyan //v8
		0.0f, 1.0f, 1.0f, a,  //v4
		0.0f, 1.0f, 1.0f, a,   //v1

		0.0f, 1.0f, 1.0f, a,  //cyan //v8
		0.0f, 1.0f, 1.0f, a,  //v1
		0.0f, 1.0f, 1.0f, a,   //v5


		0.5f, 0.0f, 0.5f, a,  //dark purple //v8
		0.5f, 0.0f, 0.5f, a,   //v5
		0.5f, 0.0f, 0.5f, a,   //v6

		0.5f, 0.0f, 0.5f, a,  //dark purple //v8
		0.5f, 0.0f, 0.5f, a,   //v7
		0.5f, 0.0f, 0.5f, a,   //v6

		0.0f,0.0f,0.205f,a, //medium blue //v7
		0.0f,0.0f,0.205f,a,  //v6
		0.0f,0.0f,0.205f,a,  //v2

		0.0f,0.0f,0.205f,a, //medium blue //v7
		0.0f,0.0f,0.205f,a,  //v2
		0.0f,0.0f,0.205f,a,  //v3

		1.0f, 0.4f, 0.7f, a,   //v4 //pink
		1.0f, 0.4f, 0.7f, a,   //v2
		1.0f, 0.4f, 0.7f, a,   //v3

		1.0f, 0.4f, 0.7f, a,   //v4 //pink
		1.0f, 0.4f, 0.7f, a,   //v1
		1.0f, 0.4f, 0.7f, a,   //v2

		0.6f, 0.4f, 0.8f, a,   //v4
		0.6f, 0.4f, 0.8f, a,   //v1
		0.6f, 0.4f, 0.8f, a    //v2

	};

	/*
	static const GLfloat cubecolor[] = {
		1.0f, 1.0f, 0.0f, a,  // κίτρινο
		1.0f, 1.0f, 0.0f, a,
		1.0f, 1.0f, 0.0f, a,

		1.0f, 1.0f, 0.0f, a,
		1.0f, 1.0f, 0.0f, a,
		1.0f, 1.0f, 0.0f, a,

		1.0f, 1.0f, 0.0f, a,
		1.0f, 1.0f, 0.0f, a,
		1.0f, 1.0f, 0.0f, a,

		1.0f, 1.0f, 0.0f, a,
		1.0f, 1.0f, 0.0f, a,
		1.0f, 1.0f, 0.0f, a,

		1.0f, 1.0f, 0.0f, a,  // κίτρινο
		1.0f, 1.0f, 0.0f, a,
		1.0f, 1.0f, 0.0f, a,

		1.0f, 1.0f, 0.0f, a,
		1.0f, 1.0f, 0.0f, a,
		1.0f, 1.0f, 0.0f, a,

		1.0f, 1.0f, 0.0f, a,
		1.0f, 1.0f, 0.0f, a,
		1.0f, 1.0f, 0.0f, a,

		1.0f, 1.0f, 0.0f, a,
		1.0f, 1.0f, 0.0f, a,
		1.0f, 1.0f, 0.0f, a,

		1.0f, 1.0f, 0.0f, a,  // κίτρινο
		1.0f, 1.0f, 0.0f, a,
		1.0f, 1.0f, 0.0f, a,

		1.0f, 1.0f, 0.0f, a,
		1.0f, 1.0f, 0.0f, a,
		1.0f, 1.0f, 0.0f, a,

		1.0f, 1.0f, 0.0f, a,
		1.0f, 1.0f, 0.0f, a,
		1.0f, 1.0f, 0.0f, a,

		1.0f, 1.0f, 0.0f, a,
		1.0f, 1.0f, 0.0f, a,
		1.0f, 1.0f, 0.0f, a

	};*/


	for (size_t i = 0; i < CUBE_COLOR_SIZE; i += 4) {
		cubecolor[i] = 1.0f;     // R
		cubecolor[i + 1] = 1.0f;   // G
		cubecolor[i + 2] = 0.0f;   // B
		cubecolor[i + 3] = a;      // A (χρησιμοποιεί τη global a = 0.4f)
	}

	//character A VBO
	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(charA), charA, GL_STATIC_DRAW);

	//VBO home buffer colors
	GLuint colorbuffer;
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(charColor), charColor, GL_STATIC_DRAW);

	//cube VBO
	GLuint cubebuffer;
	glGenBuffers(1, &cubebuffer);
	glBindBuffer(GL_ARRAY_BUFFER, cubebuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

	//VBO cube colors

	GLuint cubecolorbuffer;
	glGenBuffers(1, &cubecolorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, cubecolorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubecolor), cubecolor, GL_STATIC_DRAW);



	do {

		currentTime = glfwGetTime();
		deltaTime = float(currentTime - lastTime);
		lastTime = currentTime;

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);


		//move character A left/right
		if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) charA_x -= STEP * deltaTime;
		if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) charA_x += STEP * deltaTime;

		//rotation up on x axis 
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			rotX += 0.5f; //random to 2;
		}
		//rotation down on x axis
		if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
			rotX -= 0.5f; //random to 2;
		}

		//rotation down y axis
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
			rotY -= 0.5f; //random to 2;
		}
		//rotation up y axis
		if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
			rotY += 0.5f; //random to 2;
		}
		//zoom in
		if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS) {
			distance -= 0.2f; //random to 0.5f
		}
		//zoom out
		if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS) {
			distance += 0.2f; //random to 0.5f
		}
		//limit distance so we dont get too close
		distance = glm::max(0.5f, distance);


		//calculate camera position
		float rx = glm::radians(rotX); //gwnia panw katw  (w/z)
		float ry = glm::radians(rotY); //gwnia aristera deksia  (q/x)
		glm::vec3 cam_pos(
			distance * cos(ry) * cos(rx), //X
			distance * sin(rx), //Y
			distance * sin(ry) * cos(rx)//Z
		);

		// Camera matrix
		glm::mat4 Projection = glm::perspective(glm::radians(60.0f), 1.0f, 0.1f, 100.0f);
		glm::mat4 View = glm::lookAt(cam_pos,
			//glm::vec3(0.0f, -5.0f, 20.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f)
		);


		// Model matrix CHAR A
		glm::mat4 ModelA = glm::translate(glm::mat4(1.0f), glm::vec3(charA_x, 0.0f, 0.0f));
		glm::mat4 MVP_A = Projection * View * ModelA;
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP_A[0][0]);


		//glm::mat4 rotXMat = glm::rotate(glm::mat4(1.0f), glm::radians(rotX), glm::vec3(1.0f, 0.0f, 0.0f));
		//glm::mat4 Projection = glm::perspective(glm::radians(60.0f), 4.0f / 4.0f, 0.1f, 100.0f);
		// Camera matrix
		/*glm::mat4 View = glm::lookAt(
			glm::vec3(0.0f, -5.0f, 20.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f)
		);*/


		//glm::mat4 Model = glm::mat4(1.0f);

		//glm::mat4 MVP = Projection * View * Model;

		//glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0
		);

		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glVertexAttribPointer(
			1,
			4,                                // size
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0
		);

		// Draw triangles 
		glDrawArrays(GL_TRIANGLES, 0, 36); //12*3

		// 1st attribute buffer : cube
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, cubebuffer);
		glVertexAttribPointer(
			0,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0
		);

		// 2nd attribute buffer : colors of cube
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, cubecolorbuffer);
		glVertexAttribPointer(
			1,
			4,                                // size
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0
		);


		// Draw triangles 
		//glDrawArrays(GL_TRIANGLES, 0, 36); //12*3
		for (int i = 0; i < 5; ++i) {
			float current_X_offset = (float)i * 4;

			//metatopish ston aksona x
			glm::mat4 TranslationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(current_X_offset, 0.0f, 0.0f));
			glm::mat4 Model_Cube = TranslationMatrix;

			//ypologizoyme to mvp kai to leme ston shader?
			glm::mat4 MVP_Cube = Projection * View * Model_Cube;
			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP_Cube[0][0]);

			//draw the current cube
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}


		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);


		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

		//lastTime = currentTime;



	} // Check if the 1 key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_1) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);


	// Cleanup VBO
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &cubebuffer);
	glDeleteBuffers(1, &colorbuffer);
	glDeleteBuffers(1, &cubecolorbuffer);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

