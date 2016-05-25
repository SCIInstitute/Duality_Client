attribute vec4 position;
attribute vec3 normal;
attribute float alpha;
uniform mat4 mvpMatrix;
varying vec3 normalVarying;
varying float alphaVarying;

void main()
{	
	gl_Position = mvpMatrix * position;
	normalVarying = normal;
	alphaVarying = alpha;
}
