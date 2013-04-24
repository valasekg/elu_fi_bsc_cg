#version 130

// incoming data from pipeline

in vec3 vsout_worldPos;
in vec3 vsout_normal;
in vec4 vsout_color;

// fragment end color

out vec4 fs_out_color;

//
// lighting uniforms
//

// scene data
uniform vec3 eye_pos;
//in vec3 vsout_eyePos;

// light data
uniform vec3 light_direction = vec3( 0, -1, 0 );

uniform vec4 La = vec4(0.1f, 0.1f, 0.1f, 1);
uniform vec4 Ld = vec4(0.5f, 0.5f, 0.5f, 1);
uniform vec4 Ls = vec4(1, 1, 1, 1);

// material data
uniform vec4 Ka = vec4(1, 1, 1, 1);
uniform vec4 Kd = vec4(1, 1, 1, 1);
uniform vec4 Ks = vec4(1, 1, 1, 1);
uniform float specular_power = 16;

void main()
{
	// ambient

	// diffuse
	// useful function: normalize( vector ), clamp( x, low, high ), dot( vec_a, vec_b )

	// specular
	// useful function: reflect( incident_light_direction, surface_normal )

	fs_out_color = vec4(vsout_worldPos, 1);
}