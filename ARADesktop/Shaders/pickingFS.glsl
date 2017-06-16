#version 440 core
out vec4 colour;
in vec4 norm;
uniform vec3 pickingColour;
void main(void)
{
	colour = vec4(pickingColour,1.0f);
}
