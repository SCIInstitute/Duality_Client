attribute vec4 position;
attribute vec3 normal;
attribute vec2 tex;
uniform mat4 mvpMatrix;
varying vec3 normalVarying;
varying vec2 texVarying;

void main()
{
	gl_Position = mvpMatrix * position;
	normalVarying = normal;
	texVarying = tex;
}
