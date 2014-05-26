#version 330

uniform mat4 projectionMatrix;  
uniform mat4 viewMatrix;  
uniform mat4 modelMatrix; 

in vec3 in_Position;
in vec3 in_Color;
in vec2 in_UV;

out vec3 pass_Color;
out vec2 pass_UV;

void main() 
{            
    // Set the position of the current vertex 
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(in_Position, 1.0); 

	// Pass color values to fragment shader
	pass_Color = in_Color;

	// Pass UV co-ords to fragment shader
	pass_UV = in_UV;
}