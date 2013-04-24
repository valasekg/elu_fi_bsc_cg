#version 130

// incoming data from pipeline

in vec3 vsout_worldPos;
in vec3 vsout_normal;
in vec4 vsout_color;
in vec4 vsout_textcoord;
//in vec3 vsout_eyePos;

// fragment end color

out vec4 fs_out_color;

//
// lighting uniforms
//

// scene data
uniform vec3 eye_pos;
uniform sampler2D txt;

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
	vec4 ambient = La*Ka;

	// diffuse
	vec3 n = normalize( vsout_normal );
	vec3 l = -light_direction;
	float di = clamp( dot(n, l), 0.0f, 1.0f );
	vec4 diffuse = Ld*Kd*di;

	// specular
	vec3 e = normalize( eye_pos - vsout_worldPos );
	vec3 r = reflect( -l, n );
	float si = pow( clamp( dot(e, r), 0, 1 ), specular_power );
	vec4 specular = Ls*Ks*si;
	if ( di == 0 )
		specular = vec4(0);

	fs_out_color = texture2D(txt, vsout_textcoord.xy) * (ambient + diffuse + specular);
}