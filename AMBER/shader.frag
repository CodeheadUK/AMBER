#version 330

in vec3 pass_Color;
in vec2 pass_UV;  
in vec3 norm;

uniform sampler2D texUnit;

out vec4 out_Color; 

void main(void) 
{
	out_Color = texture(texUnit, pass_UV);
	//out_Color = vec4(pass_UV, 0.5, 1.0);  
	//out_Color = vec4(1.0, 1.0, 1.0, 1.0);  
} 
