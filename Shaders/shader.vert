#version 330 core

// stores the data in index 0 in a vec3 called aPos
layout(location = 0) in vec3 aPos;
uniform mat4 transform;

void main() {
	// denotes final position of the vertex / point
	// gl_Position is a special var so u cant change the name
	gl_Position = transform * vec4(aPos, 1.0);
}