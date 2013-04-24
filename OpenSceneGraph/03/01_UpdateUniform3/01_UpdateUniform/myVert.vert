#version 130

in vec4 osg_Vertex; 
in vec3 osg_Normal;
in vec4 osg_Color;
in vec4 osg_SecondaryColor;
in vec4 osg_MultiTexCoord0;
in vec4 osg_MultiTexCoord1;
in vec4 osg_MultiTexCoord2;
in vec4 osg_MultiTexCoord3;
in vec4 osg_MultiTexCoord4;
in vec4 osg_MultiTexCoord5;
in vec4 osg_MultiTexCoord6;
in vec4 osg_MultiTexCoord7;
in float osg_FogCoord;
in int osg_InstanceID;

out vec3 vsout_Position;
out vec3 vsout_Normal;
out vec4 vsout_Color;
out vec3 vsout_ModelPosition;

uniform mat4 osg_ModelViewProjectionMatrix;
uniform mat4 osg_ModelViewMatrix;
uniform mat4 osg_ProjectionMatrix;
uniform mat4 osg_NormalMatrix; 
uniform mat4 osg_ViewMatrixInverse;


void main()
{
	gl_Position		=  osg_ModelViewProjectionMatrix * osg_Vertex;
	vsout_Position	= (osg_ViewMatrixInverse * osg_ModelViewMatrix * osg_Vertex).xyz;
	vsout_ModelPosition = osg_Vertex.xyz;
	vsout_Normal	= (osg_NormalMatrix * vec4( osg_Normal, 0 )).xyz;
	vsout_Color		= osg_Color;	
}