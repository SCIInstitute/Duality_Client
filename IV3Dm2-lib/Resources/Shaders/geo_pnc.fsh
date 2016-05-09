precision highp float;
uniform mat4 mvpMatrix;
varying vec3 normalVarying;
varying vec4 colorVarying;
float ambient = 0.2;

void main()
{
	float diffuse = ambient + clamp(abs(dot(normalize((mvpMatrix * vec4(normalVarying.xyz, 0.0)).xyz), vec3(0.0, 0.0, 1.0))), 0.0, 1.0);
	gl_FragColor = vec4(colorVarying.r * diffuse, colorVarying.g * diffuse, colorVarying.b * diffuse, colorVarying.a);
}
