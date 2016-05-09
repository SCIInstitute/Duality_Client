precision highp float;
uniform mat4  mMVP;
precision mediump float;
varying vec2 vCoordsToFS;
uniform sampler2D sliceTexture1;
uniform sampler2D sliceTexture2;
uniform float interpolationParameter;
uniform sampler2D transferFunction;

void main()
{
	float vTexValue1 = texture2D( sliceTexture1, vCoordsToFS ).a;
	float vTexValue2 = texture2D( sliceTexture2, vCoordsToFS ).a;
	float vTexValue = vTexValue2*interpolationParameter+(1.0-interpolationParameter)*vTexValue1;
	gl_FragColor = texture2D(transferFunction, vec2(vTexValue,0.0));// + vec4(0,1.0,0,0.2);
}
