precision mediump float;
varying vec2 vCoordsToFS;
uniform sampler2D slice1;
uniform sampler2D slice2;
uniform float interpolationParameter;
uniform sampler2D tf;
uniform float oc;
uniform bool ignoreAlpha;

void main()
{
  float vTexValue1 = texture2D( slice1, vCoordsToFS ).a;
  float vTexValue2 = texture2D( slice2, vCoordsToFS ).a;
  float vTexValue = vTexValue2*interpolationParameter+(1.0-interpolationParameter)*vTexValue1;
  
  vec4 color = texture2D(tf, vec2(vTexValue, 0.0));
  if (ignoreAlpha) {
    color.a = 1.0;
  } else {
    // apply opacity correction for a single slice
    color.a = 1.0 - pow(1.0 - color.a, oc);
  }
  gl_FragColor = color;
}
