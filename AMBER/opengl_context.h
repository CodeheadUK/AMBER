#ifndef __OPENGL_CONTEXT_H
#define __OPENGL_CONTEXT_H

#include "shader.h"
#include "gl_object.h"
#include "gl_indexed_object.h"

class OpenGLContext {

public:
		OpenGLContext(void); // Default constructor
		OpenGLContext(HWND hwnd); // Constructor for creating our context given a hwnd
		~OpenGLContext(void); // Destructor for cleaning up our application
		bool create30Context(HWND hwnd); // Creation of our OpenGL 3.x context
		void setupScene(void); // All scene information can be setup here
		void reshapeWindow(int w, int h); // Method to get our window width and height on resize
		void renderScene(char* KeyPressed, long frameTicks); // Render scene (display method from previous OpenGL tutorials)

		void createSquare(void); // Method for creating our squares Vertex Array Object  
		void createHex(void); // Method for creating our map hex object  

		void advanceCam(float x);
		void strafeCam(float x);
		void rotateCam(float roll, float pitch, float yaw);
		glm::mat4 getCamMatrix(void);
		bool LoadTexture(char *TexName, GLuint TexHandle);


private:
		int winWidth;
		int winHeight;

		Shader *shader;

		GLObject obj;
		GLObject hex;
		GLIndexedObject iObj;

		unsigned int texID;

		unsigned int vaoID[2];		// Our Vertex Array Object  
  		unsigned int vboID[2];		// Our Vertex Buffer Object 
		unsigned int hexVboID[1];


		glm::mat4 projectionMatrix; // Store the projection matrix  
		glm::mat4 viewMatrix;		// Store the view matrix  
		glm::mat4 modelMatrix;		// Store the model matrix 

		// Camera Parameters
		glm::vec3 camPos, camVec, camUp;
		float camPitch, camYaw; 

		float rotAngle; 

protected:
		HGLRC	glrc;
		HDC		hdc;
		HWND	hwnd;
};

#endif