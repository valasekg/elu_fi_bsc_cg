#version 130

in vec4 osg_Vertex; 
in vec3 osg_Normal;
in vec4 osg_Color;
in vec4 osg_MultiTexCoord0;
in vec4 osg_MultiTexCoord1;
in vec4 osg_MultiTexCoord2;
in vec4 osg_MultiTexCoord3;
in vec4 osg_MultiTexCoord4;
in vec4 osg_MultiTexCoord5;
in vec4 osg_MultiTexCoord6;
in vec4 osg_MultiTexCoord7;
in vec4 osg_MultiTexCoord8;

out vec3 vsout_worldPos;
out vec3 vsout_normal;
out vec4 vsout_color;
out vec4 vsout_textcoord;
//out vec3 vsout_eyePos;

uniform mat4 osg_ModelViewProjectionMatrix;
uniform mat4 osg_ModelViewMatrix;
uniform mat4 osg_ViewMatrix;
uniform mat3 osg_NormalMatrix; 
uniform mat4 osg_ViewMatrixInverse;

void main()
{
	gl_Position		=  osg_ModelViewProjectionMatrix * osg_Vertex;
	vsout_worldPos	= (osg_ViewMatrixInverse * osg_ModelViewMatrix * osg_Vertex).xyz;
	vsout_normal	= osg_NormalMatrix * osg_Normal;
	vsout_color		= osg_Color;
	vsout_textcoord	= osg_MultiTexCoord0;
	//vsout_eyePos	= -osg_ViewMatrix[3].xyz / osg_ViewMatrix[3].w; // why don't we do this here?
}