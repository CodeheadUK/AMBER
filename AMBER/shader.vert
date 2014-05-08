uniform mat4 projectionMatrix;  
uniform mat4 viewMatrix;  
uniform mat4 modelMatrix; 

void main() {            
    // Set the position of the current vertex 
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(in_Position, 1.0); 
}