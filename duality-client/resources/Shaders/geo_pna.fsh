precision highp float;
varying vec3 normalVarying;
varying float alphaVarying;

void main()
{
	vec4 c = vec4(0.5 * (normalVarying + 1.0), alphaVarying);
	gl_FragColor = c;
}
