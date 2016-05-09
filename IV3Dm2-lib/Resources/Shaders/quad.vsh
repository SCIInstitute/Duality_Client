attribute vec4 vPosition;
attribute vec2 vTexCoords;
varying vec2 vCoordsToFS;

void main()
{
	gl_Position = vPosition;
	vCoordsToFS = vTexCoords;
}
