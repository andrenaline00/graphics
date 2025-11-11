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

	GLfloat len = 5.0f, wid = 2.5f, heig = 2.5f;

	static const GLfloat charA[] =
	{
		//pyramida , 4 triangles apo v9 pros tis 4 akres tis panw vashs
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

		//4 plevres
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

		 //katw base (z=0): 2 triangles
		-1.5f,-8.5f , 0.0f, //v4
		 1.5f,-10.0f, 0.0f, //v2
		 1.5f,-8.5f, 0.0f,  //v3

		-1.5f,-8.5f , 0.0f, //v4
		 1.5f,-10.0f, 0.0f, //v2
		-1.5f,-10.0f, 0.0f, //v1
	};
	//12 triangles -> 36 vertices (12*3)


	GLfloat a = 0.4f;
	//GLfloat a = 1.0f;
	static const GLfloat color[] = {
		// Πυραμίδα (4 τρίγωνα)
		1.0f, 0.75f, 0.80f, a, 1.0f, 0.75f, 0.80f, a, 1.0f, 0.75f, 0.80f, a,
		1.0f, 1.0f, 0.0f, a, 1.0f, 1.0f, 0.0f, a, 1.0f, 1.0f, 0.0f, a,
		0.0f, 1.0f, 0.0f, a, 0.0f, 1.0f, 0.0f, a, 0.0f, 1.0f, 0.0f, a,
		0.5f, 0.0f, 0.5f, a, 0.5f, 0.0f, 0.5f, a, 0.5f, 0.0f, 0.5f, a,

		// Πλευρές (6)
		0.68f, 0.85f, 0.90f, a, 0.68f, 0.85f, 0.90f, a, 0.68f, 0.85f, 0.90f, a,
		1.0f, 0.0f, 0.0f, a, 1.0f, 0.0f, 0.0f, a, 1.0f, 0.0f, 0.0f, a,
		1.0f, 0.65f, 0.0f, a, 1.0f, 0.65f, 0.0f, a, 1.0f, 0.65f, 0.0f, a,
		0.8f, 0.0f, 0.8f, a, 0.8f, 0.0f, 0.8f, a, 0.8f, 0.0f, 0.8f, a,
		0.4f, 1.0f, 1.0f, a, 0.4f, 1.0f, 1.0f, a, 0.4f, 1.0f, 1.0f, a,
		0.3f, 0.5f, 0.8f, a, 0.2f, 0.6f, 0.0f, a, 0.5f, 0.7f, 0.3f, a,

		// Κάτω βάση (2)
		0.05f, 0.96f, 0.12f, a, 0.39f, 0.62f, 0.36f, a, 0.67f, 0.21f, 0.46f, a,
		0.82f, 0.88f, 0.37f, a, 0.98f, 0.10f, 0.88f, a, 0.98f, 0.10f, 0.88f, a
	};
	/*static const GLfloat color[] = {
		//pyramid v9
		0.255f,  0.192f,  0.203f,a, //v9 //pink
		0.255f,  0.192f,  0.203f,a, //v8 
		0.255f,  0.192f,  0.203f,a, //v4

		0.255f,  0.255f,  0.000f,a, //v9 //yellow
		0.255f,  0.255f,  0.000f,a, //v4
		0.255f,  0.255f,  0.000f,a, //v3

		0.000f,  0.255f,  0.000f,a, //v9 //green
		0.000f,  0.255f,  0.000f,a, //v3
		0.000f,  0.255f,  0.000f,a, //v7

		0.128f,  0.000f,  0.128f,a, //v9 //purple
		0.128f,  0.000f,  0.128f,a, //v7
		0.128f,  0.000f,  0.128f,a, //v8

		//4 plevres
		0.173f,  0.216f,  0.230f,a, //v8 //light blue
		0.173f,  0.216f,  0.230f,a, //v4
		0.173f,  0.216f,  0.230f,a, //v1

		1.0f, 0.0f, 0.0f, a, //v8 //red
		1.0f, 0.0f, 0.0f, a, //v1
		1.0f, 0.0f, 0.0f, a, //v5
		

		0.255f,  0.165f,  0.000f,a, //v8 //orange
		0.255f,  0.165f,  0.000f,a, //v5
		0.255f,  0.165f,  0.000f,a, //v6

		0.204f,  0.000f,  0.204f,a, //v8 //a kind of purple
		0.204f,  0.000f,  0.204f,a, //v7
		0.204f,  0.000f,  0.204f,a, //v6

		0.102f,  0.255f,  0.255f,a, //v7 //galazoprasino
		0.102f,  0.255f,  0.255f,a, //v6
		0.102f,  0.255f,  0.255f,a, //v2

		0.302f,  0.455f,  0.848f,a, //v7
		0.225f,  0.587f,  0.040f,a, //v2
		0.517f,  0.713f,  0.338f,a, //v3

		//katw base , 2 triangles ,z=0
		0.053f,  0.959f,  0.120f,a, //v4
		0.393f,  0.621f,  0.362f,a, //v2
		0.673f,  0.211f,  0.457f,a, //v3

		0.820f,  0.883f,  0.371f,a, //v4
		0.982f,  0.099f,  0.879f,a, //v2
									//v1
	};*/

	//charA movement variables
	float charA_x = 0.0f; //charA position on x axis
	const float STEP = 1.5f; //a/2

	//camera variables
	float cam_rotX = 0.0f; //camera rotation on x axis
	float cam_rotY = 0.0f; //camera rotation on y axis
	const float ROT_SPEED= 2.0f; //rotation speed endikteiko
	const float ZOOM_SPEED = 1.0f; //zoom speed endikteiko
	float cam_distance = 20.0f; //camera distance from origin


	GLuint vertexbuffer; //vbo charA
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(charA), charA, GL_STATIC_DRAW);

	GLuint colorbuffer; //colorVBO
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);



	do {

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		/*
		float rx = glm::radians(cam_rotX);
		float ry = glm::radians(cam_rotY);
		glm::vec3 cam_pos(
			cam_distance * cos(ry)* (rx),
			cam_distance*sin(rx),
			cam_distance*sin(ry) * cos(rx)
		);*/


		glm::mat4 Projection = glm::perspective(glm::radians(60.0f), 4.0f / 4.0f, 0.1f, 100.0f);
		//Camera matrix
		glm::mat4 View = glm::lookAt(
			glm::vec3(0.0f, -5.0f, 20.0f), // Camera in World Space
			glm::vec3(0.0f, 0.0f, 0.0f), // and looks at the origin
			glm::vec3(0.0f, 1.0f, 0.0f) // Head is up
		);

		glm::mat4 Model = glm::mat4(1.0f);

		glm::mat4 MVP = Projection * View * Model;

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

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

		glDisableVertexAttribArray(0);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

		//moving charA with keys J and L
		//maybe gotta make a model matrix
		if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
				charA_x += STEP; // move x coordinate
		}
		else if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
				charA_x -= STEP; // move x coordinate
		}

		cam_rotX = glm::clamp(cam_rotX, -89.0f, 89.0f); //limit camera rotation
		//camera for rotation  (x axis)
		//for w key
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			cam_rotX -= ROT_SPEED;

		}
		//for x key
		if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
			cam_rotX += ROT_SPEED;
		}

		//camera rotation (y axis)
		//for q key
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
			cam_rotY -= ROT_SPEED;
		}
		//for z key
		if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
			cam_rotY += ROT_SPEED;
		}

		if (cam_distance < 5.0f) cam_distance = 5.0f; //stay 5 steps away 
		//camera zoom in ++
		if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS) {
			cam_distance += ZOOM_SPEED;
		}
		//zoom out --
		if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS) {
			cam_distance -= ZOOM_SPEED;
		}

	


	} 
	// Check if the 1 key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_1) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	// Cleanup VBO
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

