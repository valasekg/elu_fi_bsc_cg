#version 130

in vec3 vsout_Position;
in vec3 vsout_Normal;
in vec4 vsout_Color;
in vec3 vsout_ModelPosition;

out vec4 fs_out_color;

uniform float lightness = 1;

void main()
{
	fs_out_color = vec4(lightness*vsout_ModelPosition, 0);
}