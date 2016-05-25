attribute vec4 position;
attribute vec3 normal;
attribute vec4 color;
uniform mat4 mvpMatrix;
varying vec3 normalVarying;
varying vec4 colorVarying;

void main()
{
	gl_Position = mvpMatrix * position;
	normalVarying = normal;
	colorVarying = color;
}
