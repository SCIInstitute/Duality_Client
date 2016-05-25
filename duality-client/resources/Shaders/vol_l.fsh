precision highp float;
uniform mat4  mMVP;
precision mediump float;
varying vec2 vCoordsToFS;
uniform sampler2D sliceTexture;
uniform sampler2D transferFunction;

void main()
{
	vec4 vTexValue = texture2D( sliceTexture, vCoordsToFS);
	vec3 vNormal = normalize(mMVP * vec4((vTexValue.xyz-0.5)*2.0,0.0)).xyz;
	float fPhong = 0.2 + abs(dot(vNormal,vec3(0,0,1)));
	vec4 vColor = texture2D(transferFunction, vec2(vTexValue.a,0.0));
	gl_FragColor = vec4(vColor.r*fPhong, vColor.g*fPhong, vColor.b*fPhong, vColor.a);
}
