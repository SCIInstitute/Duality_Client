attribute vec4 vPosition;
attribute vec2 vTexCoords;
uniform mat4  mMVP;
varying vec2 vCoordsToFS;

void main()
{
	gl_Position = mMVP * vPosition;
	vCoordsToFS = vTexCoords;
}
