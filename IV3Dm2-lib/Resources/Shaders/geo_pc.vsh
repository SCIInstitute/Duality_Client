attribute vec4 position;
attribute vec4 color;
uniform mat4 mvpMatrix;
varying vec4 colorVarying;

void main()
{
	gl_Position = mvpMatrix * position;
	colorVarying = color;
}
