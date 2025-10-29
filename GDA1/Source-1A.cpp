//********************************
//Αυτό το αρχείο θα το χρησιμοποιήσετε
// για να υλοποιήσετε την άσκηση 1Α της OpenGL
//
//ΑΜ: 5309                        Όνομα: Andreana Nikou
//ΑΜ: 5379                        Όνομα: Theodora Ferentinou

//*********************************

#include <stdio.h>
#include <stdlib.h>

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <windows.h>

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

//******************
// Η LoadShaders είναι black box για σας - αγνοήστε την συνάρτηση Loadshaders

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

/*******************************************************************/
int main(void)
{
	// Initialise GLFW
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

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(900, 900, u8"Εργασία 1Α - 5309 - 5379", NULL, NULL);


	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}


	//variables
	float centerX = 0.0f;
	float centerY = 0.0f;
	const float shapeWidth = 3.0f; //from -1.5 to 1.5
	//const float leftBoundary = -15.0f + shapeSize / 2.0f; // -13.5
	//const float rightBoundary = 15.0f - shapeSize / 2.0f; // 13.5
	const float leftBoundary = -12.50f;
	const float rightBoundary = 12.50f;


	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// dark gray background
	glClearColor(34.0f/255.0f,34.0f/255.0f,34.0f/255.0f, 0.0f);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	//***********************************************
	// Οι shaders σας είναι οι 
    // ProjectVertexShader.vertexshader
    // ProjectFragmentShader.fragmentshader

	GLuint programID = LoadShaders("ProjectVertexShader.vertexshader", "ProjectFragmentShader.fragmentshader");
	
    ///////////////////////////////////////////////////////////////////////////////////////	
	/**Το παρακάτω το αγνοείτε - είναι τοποθέτηση κάμερας ***/
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 4.0f, 0.1f, 100.0f);
	// Camera matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(0, 0, 30), // Camera  in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // 
	);

	glm::mat4 Model = glm::mat4(1.0f);
	glm::mat4 MVP = Projection * View * Model; 
    ///////////////////////////////////////////////////////////////////////////////////////
	//**************************************************
	/// Για βοήθεια το πρόγραμμα αναπαριστά δυο τυχαία τρίγωνα - εσείς θα πρέπει να βάλετε κορυφές κατάλληλες 
	//  για τα δικό σας τρίγωνα.
	//Στην άσκηση αυτή δουλεύετε στις 2 διαστάσεις x,y οπότε η z συνιστώσα θα ειναι πάντα 0.0f
	// ή θα την παραλείψετε.
	/////////////////////////////////////////////////////////////////////////////////////////

	
	//character A
	GLfloat shape[] = {
		//first triangle (top)
		-1.5f, -8.0f, 0.0f,
		 1.5f, -8.0f, 0.0f,
		 0.0f, -7.25f, 0.0f,

		 //second triangle (bottom left)
		-1.5f, -8.0f, 0.0f,
		-1.5f, -9.5f, 0.0f,
		 1.5f, -8.0f, 0.0f,

		 //third triangle (bottom right)
		 1.5f, -8.0f, 0.0f,
		 1.5f, -9.5f, 0.0f,
		-1.5f, -9.5f, 0.0f
	};



	
	GLfloat starShape[] = {
		//base
		//first triangle 
		-0.25f , 0.0f , 0.0f,
		0.25f , 0.0f , 0.0f ,
		-0.25f , -0.5f , 0.0f,

		//second triangle
		
		0.25f , 0.0f , 0.0f,
		0.25f , -0.5f , 0.0f ,
		-0.25f , -0.5f , 0.0f,
	//};


	//GLfloat starShape[] = {
		//rest
		//first triangle 
		0.0f , 1.5f , 0.0f,
		-0.25f , 0.0f , 0.0f ,
		0.25f , 0.0f , 0.0f,
		//second triangle
		-1.75f , -0.25f , 0.0f,
		-0.25f , 0.0f , 0.0f ,
		-0.25f , -0.5f , 0.0f,
		//third triangle
		-0.25f , -0.5f , 0.0f,
		0.0f , -2.0f , 0.0f ,
		0.25f , -0.5f , 0.0f,
		//fourth triangle
		0.25f , 0.0f , 0.0f,
		0.25f , -0.5f , 0.0f ,
		1.75f , -0.25f , 0.0f
	};



	//gotta make VBO and VA0 for star shape 




	//VBO shape
	GLuint vbuffer;
	glGenBuffers(1, &vbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(shape), shape, GL_STATIC_DRAW);

	//VBO star
	GLuint vbufferstar;
	glGenBuffers(1, &vbufferstar);
	glBindBuffer(GL_ARRAY_BUFFER, vbufferstar);
	glBufferData(GL_ARRAY_BUFFER, sizeof(starShape), starShape, GL_STATIC_DRAW);

	do {

		// Clear the screen 
		glClear(GL_COLOR_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);  /// Αυτό αφορά την κάμερα  - το αγνοείτε

		glGenBuffers(1, &vbuffer); 
		glBindBuffer(GL_ARRAY_BUFFER, vbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(shape), shape, GL_STATIC_DRAW);

		

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vbuffer);
		
		
		glVertexAttribPointer(
			0,                  // attribute 0, must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 3*3); // 3 vertices for each triangle -> 6 vertices total for 2 triangles



		glGenBuffers(1, &vbufferstar);
		glBindBuffer(GL_ARRAY_BUFFER, vbufferstar);
		glBufferData(GL_ARRAY_BUFFER, sizeof(starShape), starShape, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, vbufferstar);

		//glDisableVertexAttribArray(0);

		glVertexAttribPointer(
			0,                  // attribute 0, must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 3 * 6); // 3 vertices for each triangle -> 6 vertices total for 2 triang
		
		glDisableVertexAttribArray(0);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();


		//moving the shape with keys J and L
		//do not get out of window ,left  and right boundary check
		if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
			if (shape[3] < rightBoundary && shape[6] < rightBoundary && shape[9] < rightBoundary) { // boundary check for right side
				for (int i = 0; i < sizeof(shape) / sizeof(shape[0]); i += 3) {
					shape[i] += 0.01f; // move x coordinate
				}
			}
		}
		else if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
			if (shape[0] > leftBoundary && shape[12] > leftBoundary && shape[15] > leftBoundary) { // boundary check for left side
				for (int i = 0; i < sizeof(shape) / sizeof(shape[0]); i += 3) {
					shape[i] -= 0.01f; // move x coordinate
				}
			}
		}
			



	} 
	while (glfwGetKey(window, GLFW_KEY_1) != GLFW_PRESS &&  glfwWindowShouldClose(window) == 0);




	// Cleanup VBO
	glDeleteBuffers(1, &vbuffer);
	glDeleteBuffers(1, &vbufferstar);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

