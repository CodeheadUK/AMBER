#include "stdafx.h"
#include "headers.h"
#include "opengl_context.h"

OpenGLContext::OpenGLContext(void) {

}

OpenGLContext::OpenGLContext(HWND window) {
	create30Context(window);
}

/** 
Clean up rendering context 
Release the device context from the current window. 
*/
OpenGLContext::~OpenGLContext(void) {

	wglMakeCurrent(hdc, 0); // Remove the rendering context from our device context
	wglDeleteContext(glrc); // Delete our rendering context

	ReleaseDC(hwnd, hdc); // Release the device context from our window
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

	obj.BuildSquare(); // Create our square 
	//createHex();

	projectionMatrix = glm::perspective(60.0f, (float)winWidth / (float)winHeight, 0.1f, 100.f);  // Create our perspective projection matrix  
	
	camUp = glm::vec3(0.0f, 1.0f, 0.0f);
	//camPos = glm::vec3(0.0f, 10.0f, -15.0f);
	camPos = glm::vec3(0.0f, 0.0f, -5.0f);
	camVec = glm::vec3(0.0f, 0.0f, 1.0f);
	camPitch = 0.0f;
	camYaw = 0.0f;

	rotAngle = 0.0f;


}  

/** 
createSquare is used to create the Vertex Array Object which will hold our square. We will 
be hard coding in the vertices for the square, which will be done in this method. 
*/  
void OpenGLContext::createSquare(void) {  
  float* vertices = new float[18];  // Vertices for our square  
  float* cols = new float[18]; // Colours!
  
vertices[0] = -0.5; vertices[1] = -0.5; vertices[2] = 0.0; // Bottom left corner  

cols[0] = 1.0f; 
cols[1] = 0.0f;
cols[2] = 0.0f;

vertices[3] = -0.5; vertices[4] = 0.5; vertices[5] = 0.0; // Top left corner  

cols[3] = 0.0f; 
cols[4] = 1.0f;
cols[5] = 0.0f;

vertices[6] = 0.5; vertices[7] = 0.5; vertices[8] = 0.0; // Top Right corner 

cols[6] = 0.0f; 
cols[7] = 0.0f;
cols[8] = 1.0f;
  
vertices[9] = 0.5; vertices[10] = -0.5; vertices[11] = 0.0; // Bottom right corner  

cols[9] = 1.0f; 
cols[10] = 1.0f;
cols[11] = 0.0f;

vertices[12] = -0.5; vertices[13] = -0.5; vertices[14] = 0.0; // Bottom left corner  

cols[12] = 1.0f; 
cols[13] = 0.0f;
cols[14] = 1.0f;

vertices[15] = 0.5; vertices[16] = 0.5; vertices[17] = 0.0; // Top Right corner  
 
cols[15] = 0.0f; 
cols[16] = 1.0f;
cols[17] = 1.0f;

glGenVertexArrays(1, &vaoID[0]); // Create our Vertex Array Object  
glBindVertexArray(vaoID[0]); // Bind our Vertex Array Object so we can use it  
  
glGenBuffers(2, &vboID[0]); // Generate our Vertex Buffer Objects

glBindBuffer(GL_ARRAY_BUFFER, vboID[0]); // Bind our Vertex Buffer Object  
glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(GLfloat), vertices, GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW  
glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer 

glBindBuffer(GL_ARRAY_BUFFER, vboID[1]); // Bind our second Vertex Buffer Object 
glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(GLfloat), cols, GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW 
glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer  
  
glEnableVertexAttribArray(1); // Enable the second vertex attribute array
  
//glEnableVertexAttribArray(0); // Disable our Vertex Array Object  
glBindVertexArray(0); // Disable our Vertex Buffer Object 

delete [] vertices; // Delete our vertices from memory
delete [] cols; // Delete our vertices from memory  
}  

void OpenGLContext::createHex(void)
{
	float* vertices = new float[54];  // Vertices for our hex

	vertices[0]  =   0.0f; vertices[1] = 0.0f; vertices[2] =   0.0f;     // pt0
	vertices[3]  = -10.0f; vertices[4] = 0.0f; vertices[5] =  17.32f;    // pt23
	vertices[6]  =  10.0f; vertices[7] = 0.0f; vertices[8] =  17.32f;    // pt8

	vertices[9]  =   0.0f; vertices[10]  = 0.0f; vertices[11] =   0.0f;  // pt0
	vertices[12] =  10.0f; vertices[13]  = 0.0f; vertices[14] =  17.32f; // pt8
	vertices[15] =  20.0f; vertices[16]  = 0.0f; vertices[17] =   0.0f;  // pt11

	vertices[18] =   0.0f; vertices[19] = 0.0f; vertices[20]  =   0.0f;  // pt0
	vertices[21] =  20.0f; vertices[22] = 0.0f; vertices[23] =   0.0f;  // pt11
	vertices[24] =  10.0f; vertices[25] = 0.0f; vertices[26] = -17.32f; // pt14

	vertices[27] =   0.0f; vertices[28] = 0.0f; vertices[29] =   0.0f;  // pt0
	vertices[30] =  10.0f; vertices[31] = 0.0f; vertices[32] = -17.32f; // pt14
	vertices[33] = -10.0f; vertices[34] = 0.0f; vertices[35] = -17.32f; // pt17

	vertices[36] =   0.0f; vertices[37] = 0.0f; vertices[38] =   0.0f;  // pt0
	vertices[39] = -10.0f; vertices[40] = 0.0f; vertices[41] = -17.32f; // pt17
	vertices[42] = -20.0f; vertices[43] = 0.0f; vertices[44] =   0.0f;  // pt20

	vertices[45] =   0.0f; vertices[46] = 0.0f; vertices[47] =   0.0f;  // pt0
	vertices[48] = -20.0f; vertices[49] = 0.0f; vertices[50] =   0.0f;  // pt20
	vertices[51] = -10.0f; vertices[52] = 0.0f; vertices[53] =  17.32f; // pt23

	glGenVertexArrays(1, &vaoID[1]); // Create our Vertex Array Object  
	glBindVertexArray(vaoID[1]); // Bind our Vertex Array Object so we can use it  
  
	glGenBuffers(1, hexVboID); // Generate our Vertex Buffer Object  
	glBindBuffer(GL_ARRAY_BUFFER, hexVboID[0]); // Bind our Vertex Buffer Object  
	glBufferData(GL_ARRAY_BUFFER, 54 * sizeof(GLfloat), vertices, GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW  
  
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer  
  
	//glEnableVertexAttribArray(0); // Disable our Vertex Array Object  
	glBindVertexArray(0); // Disable our Vertex Buffer Object 

	delete [] vertices; // Delete our vertices from memory
}

void OpenGLContext::advanceCam(float x)
{
	camPos +=  camVec * x;
}

void OpenGLContext::strafeCam(float x)
{
	camPos +=  glm::cross(camVec * x, camUp);
}

void OpenGLContext::rotateCam(float roll, float pitch, float yaw)
{
	camPitch += pitch;
	camYaw	 += yaw;
}

glm::mat4 OpenGLContext::getCamMatrix(void)
{
	glm::vec3 camTgt(0.0f);
	camTgt[2] += 1.0f;

	camTgt = glm::rotateX(camTgt, camPitch); 
	camVec = glm::rotateY(camTgt, camYaw); 

	return glm::lookAt(camPos, camPos + camVec, camUp);
}

/** 
reshapeWindow is called every time our window is resized, and it sets our windowWidth and windowHeight 
so that we can set our viewport size. 
*/  
void OpenGLContext::reshapeWindow(int w, int h) {  
	winWidth = w; // Set the window width  
	winHeight = h; // Set the window height  
	projectionMatrix = glm::perspective(60.0f, (float)winWidth / (float)winHeight, 0.1f, 100.f);  // Create our perspective projection matrix  
}  


/** 
renderScene will contain all our rendering code. 
 
The first thing we are going to do is set our viewport size to fill the entire window. 
 
Next up we are going to clear our COLOR, DEPTH and STENCIL buffers to avoid overlapping 
of renders. 
 
Any of your other rendering code will go here. 
 
Finally we are going to swap buffers. 
*/  
void OpenGLContext::renderScene(char* KeyPressed, long frameTicks) 
{
	char dmsg[256];
	sprintf(dmsg,"FrameTime %d\n", frameTicks);
	//OutputDebugString(dmsg);

	rotAngle += 0.1f * frameTicks;

	// Handle key presses
	if(KeyPressed[VK_LEFT])
	{
		strafeCam(-0.01f * frameTicks);
	}

	if(KeyPressed[VK_RIGHT])
	{
		strafeCam( 0.01f * frameTicks);
	}

	if(KeyPressed[VK_UP])
	{
		advanceCam( 0.01f * frameTicks);
	}

	if(KeyPressed[VK_DOWN])
	{
		advanceCam(-0.01f * frameTicks);
	}

	if(KeyPressed[188])
	{
		rotateCam( 0.0f, 0.0f, 0.01f * frameTicks);
	}

	if(KeyPressed[190])
	{
		rotateCam( 0.0f, 0.0f, -0.01f * frameTicks);
	}

	if(KeyPressed[35])
	{
		rotateCam( 0.0f, 0.01f * frameTicks, 0.0f);
	}

	if(KeyPressed[36])
	{
		rotateCam( 0.0f,-0.01f * frameTicks, 0.0f);
	}


	glViewport(0, 0, winWidth, winHeight); // Set the viewport size to fill the window  
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // Clear required buffers  

	//viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.f)); // Create our view matrix which will translate us back 5 units  
	//viewMatrix = glm::translate(glm::mat4(1.0f), camera); // Create our view matrix based on the camera position
	viewMatrix = getCamMatrix();
	modelMatrix = glm::rotate(rotAngle, camUp);  // Create our model matrix which will halve the size of our model  

	shader->bind();

	int projectionMatrixLocation = glGetUniformLocation(shader->id(), "projectionMatrix"); // Get the location of our projection matrix in the shader  
	int viewMatrixLocation = glGetUniformLocation(shader->id(), "viewMatrix"); // Get the location of our view matrix in the shader  
	int modelMatrixLocation = glGetUniformLocation(shader->id(), "modelMatrix"); // Get the location of our model matrix in the shader 

	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]); // Send our projection matrix to the shader  
	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]); // Send our view matrix to the shader  

	glm::vec4 v(0.01f, 0.0f, 0.0f, 0.0f);

	//modelMatrix = glm::translate(&modelMatrix,&v); 

	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]); // Send our model matrix to the shader  


//	glBindVertexArray(vaoID[0]); // Bind our Vertex Array Object  
//	glDrawArrays(GL_TRIANGLES, 0, 6); // Draw our square  
	obj.Render();

	//glBindVertexArray(vaoID[1]); // Bind the Hex Vertex Array
	//glDrawArrays(GL_TRIANGLES, 0, 18); // Draw our hex 

  
	glBindVertexArray(0); // Unbind our Vertex Array Object  

	shader->unbind();

	SwapBuffers(hdc); // Swap buffers so we can see our rendering  
} 
