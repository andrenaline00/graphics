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
	static const GLfloat color[] = {
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
		0.173f,  0.216f,  0.230f,a, //v8 //light blue
		0.173f,  0.216f,  0.230f,a, //v4
		0.173f,  0.216f,  0.230f,a, //v1
		0.971f,  0.572f,  0.833f,a, //v8
		0.140f,  0.616f,  0.489f,a, //v1
		0.997f,  0.513f,  0.064f,a, //v5
		0.945f,  0.719f,  0.592f,a, //v8
		0.543f,  0.021f,  0.978f,a, //v5
		0.279f,  0.317f,  0.505f,a, //v6
		0.167f,  0.620f,  0.077f,a, //v8
		0.347f,  0.857f,  0.137f,a, //v7
		0.055f,  0.953f,  0.042f,a, //v6
		0.714f,  0.505f,  0.345f,a, //v7
		0.783f,  0.290f,  0.734f,a, //v6
		0.722f,  0.645f,  0.174f,a, //v2
		0.302f,  0.455f,  0.848f,a, //v7
		0.225f,  0.587f,  0.040f,a, //v2
		0.517f,  0.713f,  0.338f,a, //v3
		0.053f,  0.959f,  0.120f,a, //v4
		0.393f,  0.621f,  0.362f,a, //v2
		0.673f,  0.211f,  0.457f,a, //v3
		0.820f,  0.883f,  0.371f,a, //v
		0.982f,  0.099f,  0.879f,a, //v
	};


	//character A VBO
	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(charA), charA, GL_STATIC_DRAW);

	GLuint colorbuffer;
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);

	//cube VBO
	GLuint cubebuffer;
	glGenBuffers(1, &cubebuffer);
	glBindBuffer(GL_ARRAY_BUFFER, cubebuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

	do {

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		glm::mat4 Projection = glm::perspective(glm::radians(60.0f), 4.0f / 4.0f, 0.1f, 100.0f);
		// Camera matrix
		glm::mat4 View = glm::lookAt(
			glm::vec3(0.0f, -5.0f, 20.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f)
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

		// attribute buffer : cube
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
		/*
		computeMatricesFromInputs();
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();
		glm::mat4 ModelMatrix = glm::mat4(1.0);
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		*/
		/*
		//x axis
		if (glfwGetKey(window,GLFW_KEY_W) == GLFW_PRESS) {
			position += direction * deltaTime * speed;
		}

		if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
			position -= direction * deltaTime * speed;
		}
		// y axis
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
			position += right * deltaTime * speed;
		}

		if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
			position -= right * deltaTime * speed;
		}
		//zoom in/zoom out
		if (glfwGetKey(window, GLFW_KEY_+) == GLFW_PRESS) {
			position -= right * deltaTime * speed;
		}

		if (glfwGetKey(window, GLFW_KEY_-) == GLFW_PRESS) {
			position -= right * deltaTime * speed;
		}
		*/

		glDisableVertexAttribArray(0);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the 1 key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_1) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	// Cleanup VBO
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &cubebuffer);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

