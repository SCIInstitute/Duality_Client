precision highp float;
uniform mat4  mMVP;
precision mediump float;
varying vec2 vCoordsToFS;
uniform sampler2D sliceTexture;
uniform sampler2D transferFunction;

void main()
{
	float vTexValue = texture2D( sliceTexture, vCoordsToFS ).a;
	gl_FragColor = texture2D(transferFunction, vec2(vTexValue,0.0));
}
