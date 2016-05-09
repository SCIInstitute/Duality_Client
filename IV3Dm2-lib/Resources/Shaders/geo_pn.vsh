attribute vec4 position;
attribute vec3 normal;
uniform mat4 mvpMatrix;
varying vec3 normalVarying;

void main()
{
	gl_Position = mvpMatrix * position;
	normalVarying = normal;
}
