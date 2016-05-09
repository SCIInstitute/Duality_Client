attribute vec4 position;
//attribute vec3 normal;
attribute vec2 tex;
attribute float alpha;
uniform mat4 mvpMatrix;
//varying vec3 normalVarying;
varying vec2 texVarying;
varying float alphaVarying;

void main()
{
	gl_Position = mvpMatrix * position;
//	normalVarying = normal;
	texVarying = tex;
	alphaVarying = alpha;
}
