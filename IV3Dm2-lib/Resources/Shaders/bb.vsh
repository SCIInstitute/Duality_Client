attribute vec4 vPosition;
attribute vec4 vColor;
uniform mat4  mMVP;
varying vec4 vColorToFS;

void main()
{
	gl_Position = mMVP * vPosition;
	vColorToFS = vColor;
}
