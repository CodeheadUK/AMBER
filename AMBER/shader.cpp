#include "stdafx.h"

#include "headers.h"

#include "shader.h"

using namespace std;

static char* textFileRead(const char *fileName) 
{
	char* text;
    
	if (fileName != NULL)
	{
		FILE *file = fopen(fileName, "rt");
        
		if (file != NULL) {
			fseek(file, 0, SEEK_END);
			int count = ftell(file);
			rewind(file);
            
			if (count > 0)
			{
				text = (char*)malloc(sizeof(char) * (count + 1));
				count = fread(text, sizeof(char), count, file);
				text[count] = '\0';
			}

			fclose(file);
		}
	}

	return text;
}

Shader::Shader() {
    
}

Shader::Shader(const char *vsFile, const char *fsFile) {
    init(vsFile, fsFile);
}

void Shader::init(const char *vsFile, const char *fsFile)
{

	glBindAttribLocation(shader_id, 0, "in_Position"); // Bind a constant attribute location for positions of vertices  
	glBindAttribLocation(shader_id, 1, "in_Color"); // Bind another constant attribute location, this time for color

    shader_vp = glCreateShader(GL_VERTEX_SHADER);
    shader_fp = glCreateShader(GL_FRAGMENT_SHADER);
    
    const char* vsText = textFileRead(vsFile);
    const char* fsText = textFileRead(fsFile);    
    
    if (vsText == NULL || fsText == NULL) {
        cerr << "Either vertex shader or fragment shader file not found." << endl;
        return;
    }
    
    glShaderSource(shader_vp, 1, &vsText, 0);
    glShaderSource(shader_fp, 1, &fsText, 0);
    
    glCompileShader(shader_vp);
    glCompileShader(shader_fp);
    
    shader_id = glCreateProgram();
    glAttachShader(shader_id, shader_fp);
    glAttachShader(shader_id, shader_vp);
    glLinkProgram(shader_id);
}

Shader::~Shader() {
    glDetachShader(shader_id, shader_fp);
    glDetachShader(shader_id, shader_vp);
    
    glDeleteShader(shader_fp);
    glDeleteShader(shader_vp);
    glDeleteProgram(shader_id);
}

unsigned int Shader::id() {
    return shader_id;
}

void Shader::bind() {
    glUseProgram(shader_id);
}

void Shader::unbind() {
    glUseProgram(0);
}