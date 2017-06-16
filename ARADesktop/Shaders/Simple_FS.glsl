#version 440 core                                                                   
out vec4 colour_out;
uniform sampler2D tex;
uniform bool isTextured, hasMaterial;
uniform vec3 material;
in VS_OUT
{
	vec2 uv;
	vec4 normals;
} vs_in;
void main(void)
{
	vec4 colour;


	if (isTextured) {
		colour = texture(tex, vs_in.uv);
	}
	else if (hasMaterial) 
	{
		colour = vec4(material, 1.0f);
	}
	else{
		colour = 0.5f * (vs_in.normals + vec4(1.0f));
	}

	colour_out = colour;
}                                         