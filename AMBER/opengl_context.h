#ifndef __OPENGL_CONTEXT_H
#define __OPENGL_CONTEXT_H

#include "shader.h"

class OpenGLContext {

public:
		OpenGLContext(void); // Default constructor
		OpenGLContext(HWND hwnd); // Constructor for creating our context given a hwnd
		~OpenGLContext(void); // Destructor for cleaning up our application
		bool create30Context(HWND hwnd); // Creation of our OpenGL 3.x context
		void setupScene(void); // All scene information can be setup here
		void reshapeWindow(int w, int h); // Method to get our window width and height on resize
		void renderScene(void); // Render scene (display method from previous OpenGL tutorials)

private:
		int winWidth;
		int winHeight;

		Shader *shader;

protected:
		HGLRC	glrc;
		HDC		hdc;
		HWND	hwnd;
};

#endif