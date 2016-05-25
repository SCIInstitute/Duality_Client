precision highp float;
uniform mat4 mvpMatrix;
varying vec3 normalVarying;
varying vec2 texVarying;
uniform sampler2D texture;
float ambient = 0.2;

void main()
{
	float diffuse = ambient + clamp(abs(dot(normalize((mvpMatrix * vec4(normalVarying.xyz, 0.0)).xyz), vec3(0.0, 0.0, 1.0))), 0.0, 1.0);
	vec4 color = texture2D(texture, texVarying);
	gl_FragColor = vec4(color.r * diffuse, color.g * diffuse, color.b * diffuse, color.a);
}
