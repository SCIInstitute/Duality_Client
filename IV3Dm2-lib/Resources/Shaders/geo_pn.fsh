precision highp float;
varying vec3 normalVarying;

void main()
{
	vec4 c = vec4(0.5 * (normalVarying + 1.0), 1.0);
	gl_FragColor = c;
}
