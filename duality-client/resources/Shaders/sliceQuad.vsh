attribute vec4 vPosition;
attribute vec2 vTexCoords;
uniform mat4 mvpMatrix;
varying vec2 vCoordsToFS;

void main()
{
	gl_Position = mvpMatrix * vPosition;
	vCoordsToFS = vTexCoords;
}
