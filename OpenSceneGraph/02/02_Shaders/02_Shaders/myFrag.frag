#version 130

in vec3 vsout_Position;
in vec3 vsout_Normal;
in vec4 vsout_Color;

out vec4 fs_out_color;

void main()
{
	//vec3 norm
	fs_out_color = vec4(vsout_Position, 0);
}