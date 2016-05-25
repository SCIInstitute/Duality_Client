precision highp float;
//varying vec3 normalVarying;
varying vec2 texVarying;
varying float alphaVarying;
uniform sampler2D texture;

void main()
{
	vec4 c = vec4(texture2D(texture, texVarying).xyz, alphaVarying);
	gl_FragColor = c;
}
