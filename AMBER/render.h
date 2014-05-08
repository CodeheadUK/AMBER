// This code that will be executed by the OpenGL base app's render thread

int TestRender()
{
	

	return 1;
}

bool Render(LPVOID lpParam)
 {
  HGLRC glRC;

  // Render Pointer
  int (*rPtr)(void);

  // Assign render function pointer to start function
  rPtr = &TestRender;

  // Re-aquire the context as we are in a different thread
  glRC=wglCreateContext(GLDC);
  wglMakeCurrent(GLDC,glRC); 

  glClearColor(0,0,0,0);
  glClear(GL_COLOR_BUFFER_BIT);

  // This loop will run until Esc is pressed
   while(RunLevel)
    {
     if(Keys[27]) // Esc Key
      RunLevel=0;

	 rPtr();
	 	   

     SwapBuffers(GLDC);
    }

  return 0;
 }

