#include "stdafx.h"
#include "headers.h"
#include "opengl_context.h"

OpenGLContext::OpenGLContext(void) {
	OutputDebugString("GL Constructor complete\n");
}

OpenGLContext::OpenGLContext(HWND window) {
	create30Context(window);
	OutputDebugString("GL Constructor complete\n");
}

/** 
Destructor for our OpenGLContext class which will clean up our rendering context 
and release the device context from the current window. 
*/
OpenGLContext::~OpenGLContext(void) {

	wglMakeCurrent(hdc, 0); // Remove the rendering context from our device context
	wglDeleteContext(glrc); // Delete our rendering context

	ReleaseDC(hwnd, hdc); // Release the device context from our window

	OutputDebugString("GL Destructor complete\n");
}


/**
create30Context creates an OpenGL context and attaches it to the window provided by
the HWND. This method currently creates an OpenGL 3.2 context by default, but will default
to an OpenGL 2.1 capable context if the OpenGL 3.2 context cannot be created.
*/
bool OpenGLContext::create30Context(HWND window) {

	hwnd = window; // Set the HWND for our window
	hdc = GetDC(hwnd); // Get the device context for our window

	PIXELFORMATDESCRIPTOR pfd; // Create a new PIXELFORMATDESCRIPTOR (PFD)  
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR)); // Clear our  PFD  
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR); // Set the size of the PFD to the size of the class  
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW; // Enable double buffering, opengl support and drawing to a window  
	pfd.iPixelType = PFD_TYPE_RGBA; // Set our application to use RGBA pixels  
	pfd.cColorBits = 32; // Give us 32 bits of color information (the higher, the more colors)  
	pfd.cDepthBits = 32; // Give us 32 bits of depth information (the higher, the more depth levels)  
	pfd.iLayerType = PFD_MAIN_PLANE; // Set the layer of the PFD  
  
	int nPixelFormat = ChoosePixelFormat(hdc, &pfd); // Check if our PFD is valid and get a pixel format back  
	if (nPixelFormat == 0) // If it fails  
		return false;  

	bool bResult = SetPixelFormat(hdc, nPixelFormat, &pfd); // Try and set the pixel format based on our PFD  
	if (!bResult) // If it fails  
		return false; 
	
	HGLRC tempOpenGLContext = wglCreateContext(hdc); // Create an OpenGL 2.1 context for our device context  
	wglMakeCurrent(hdc, tempOpenGLContext); // Make the OpenGL 2.1 context current and active  
 
	GLenum error = glewInit(); // Enable GLEW  
	if (error != GLEW_OK) // If GLEW fails  
		return false; 

	int attributes[] = {  
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3, // Set the MAJOR version of OpenGL to 3  
		WGL_CONTEXT_MINOR_VERSION_ARB, 2, // Set the MINOR version of OpenGL to 2  
		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB, // Set our OpenGL context to be forward compatible  
		0  
	};

	if (wglewIsSupported("WGL_ARB_create_context") == 1) { // If the OpenGL 3.x context creation extension is available  
		glrc = wglCreateContextAttribsARB(hdc, NULL, attributes); // Create and OpenGL 3.x context based on the given attributes  
		wglMakeCurrent(NULL, NULL); // Remove the temporary context from being active  
		wglDeleteContext(tempOpenGLContext); // Delete the temporary OpenGL 2.1 context  
		wglMakeCurrent(hdc, glrc); // Make our OpenGL 3.0 context current  
	}  
	else {  
		glrc = tempOpenGLContext; // If we didn't have support for OpenGL 3.x and up, use the OpenGL 2.1 context  
	} 

	int glVersion[2] = {-1, -1}; // Set some default values for the version  
	glGetIntegerv(GL_MAJOR_VERSION, &glVersion[0]); // Get back the OpenGL MAJOR version we are using  
	glGetIntegerv(GL_MINOR_VERSION, &glVersion[1]); // Get back the OpenGL MAJOR version we are using  
  
	char msg[256];

	sprintf(msg, "Using OpenGL %d.%d\n",glVersion[0],glVersion[1]);
	OutputDebugString(msg);

	return true; // We have successfully created a context, return true
}

/** 
setupScene will contain anything we need to setup before we render 
*/  
void OpenGLContext::setupScene(void) {  
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Set the clear color based on Microsofts CornflowerBlue (default in XNA)  

	shader = new Shader("shader.vert", "shader.frag");
}  

/** 
reshapeWindow is called every time our window is resized, and it sets our windowWidth and windowHeight 
so that we can set our viewport size. 
*/  
void OpenGLContext::reshapeWindow(int w, int h) {  
	winWidth = w; // Set the window width  
	winHeight = h; // Set the window height  
}  


/** 
renderScene will contain all our rendering code. 
 
The first thing we are going to do is set our viewport size to fill the entire window. 
 
Next up we are going to clear our COLOR, DEPTH and STENCIL buffers to avoid overlapping 
of renders. 
 
Any of your other rendering code will go here. 
 
Finally we are going to swap buffers. 
*/  
void OpenGLContext::renderScene(void) 
{
	glViewport(0, 0, winWidth, winHeight); // Set the viewport size to fill the window  
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // Clear required buffers  

	shader->bind();



	shader->unbind();

	SwapBuffers(hdc); // Swap buffers so we can see our rendering  
} 
