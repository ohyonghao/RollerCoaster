#version 330
/*uniform sampler2D qt_Texture0;
varying vec4 qt_TexCoord0;

void main(void)
{
    gl_FragColor = texture2D(qt_Texture0, qt_TexCoord0.st);
}*/

uniform vec4 color;

out vec4 fragColor;

void main(void)
{
    fragColor = color;
}
