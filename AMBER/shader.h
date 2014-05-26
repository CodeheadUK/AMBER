#ifndef __SHADER_H__
#define __SHADER_H__

#include "stdafx.h"

#include "headers.h"
#include <stdlib.h>

#include <string>
#include <fstream>

class Shader {
public:
	Shader();
	Shader(const char *vsFile, const char *fsFile);
	~Shader();
    
	void init(const char *vsFile, const char *fsFile);
    
	void bind();
	void unbind();
    
	unsigned int id();

	unsigned int texUnit;
    
private:
	unsigned int shader_id;
	unsigned int shader_vp;
	unsigned int shader_fp;

	bool inited; // Whether or not we have initialized the shader
};
#endif