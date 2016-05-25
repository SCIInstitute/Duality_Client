attribute vec4 position;
attribute vec2 tex;
uniform mat4 mvpMatrix;
varying vec2 texVarying;

void main()
{
	gl_Position = mvpMatrix * position;
	texVarying = tex;
}
