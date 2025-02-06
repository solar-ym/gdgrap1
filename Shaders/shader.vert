#version 330 core

layout(location = 0) in vec3 aPos;

//Transformation matrix
uniform mat4 transform;

//Projection matrix
uniform mat4 projection;

//View matrix
uniform mat4 view;

void main(){
	//gl_Position = transform * vec4(aPos, 1.0);
	//Saving the above for my rabbit submission
	gl_Position = projection * view * transform * vec4(aPos, 1.0);
}