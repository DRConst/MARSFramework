#version 440 core
  
layout(location = 0) in vec4 position;
layout(location = 1) in vec4 normals;
layout(location = 2) in vec2 uv;
uniform mat4 mvpMatrix;
out VS_OUT
{
	vec2 uv;
	vec4 normals;	
} vs_out;

void main()
{
	//gl_Position = m_matrix * v_matrix * proj_matrix * position;
	gl_Position = mvpMatrix * position;
	vs_out.normals = normals;
	vs_out.uv = uv;
}

