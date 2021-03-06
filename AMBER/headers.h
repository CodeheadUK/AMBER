#ifndef __HEADERS_H__
#define __HEADERS_H__

#include "stdafx.h"
#include "GL/glew.h"
//#include "GL/glxew.h"
#include "GL/wglew.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include "glm/glm.hpp"
//#include "glm/gtc/matrix_projection.hpp"  
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/rotate_vector.hpp"


#include <iostream>

#include "opengl_context.h"

#include "AMBER_Defs.h"
#include "shader.h"
#include "gl_object.h"
#include "gl_indexed_object.h"


#pragma comment (lib , "opengl32.lib") // Makes VC link the gl libs, 
#pragma comment (lib , "glu32.lib") // other compliers will have to do it manually
#pragma comment (lib , "glew32.lib")

#endif