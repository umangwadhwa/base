varying vec2 uv;
uniform sampler2D texture_sample;

void main()
{
    gl_FragColor = texture2D(texture_sample, uv);
}