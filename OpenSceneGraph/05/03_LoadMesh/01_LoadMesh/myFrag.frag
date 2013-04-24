#version 330 core

in vec3 vs_out_col;
in vec2 vs_out_tex0;
out vec4 fs_out_col;

uniform sampler2D texture;

void main()
{
	fs_out_col = vec4(vs_out_col, 1) * texture2D(texture, vs_out_tex0.st);
}