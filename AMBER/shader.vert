#version 150 core

uniform mat4 projectionMatrix;  
uniform mat4 viewMatrix;  
uniform mat4 modelMatrix; 

in vec3 in_Position;
in vec3 in_Color;
out vec3 pass_Color;

void main() 
{            
    // Set the position of the current vertex 
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(in_Position, 1.0); 

	// Color
	pass_Color = in_Color;
}