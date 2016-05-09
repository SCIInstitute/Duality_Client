precision highp float;
uniform mat4 mvpMatrix;
varying vec4 colorVarying;

void main()
{
	gl_FragColor = colorVarying;
}
