precision mediump float;
varying highp vec2 coord;
uniform sampler2D src;
uniform lowp float qt_Opacity;
uniform lowp vec2 iResolution;
uniform lowp float iTime;
void main() {
    vec2 uv = coord.xy / iResolution.xy;

    //get the colour
    float xCol = (uv.x - (iTime / 8.0)) * 3.0;
    xCol = mod(xCol, 3.0);
    vec3 horColour = vec3(0.25, 0.25, 0.25);

    if (xCol < 1.0) {
        horColour.r += 1.0 - xCol;
        horColour.g += xCol;
    }
    else if (xCol < 2.0) {

        xCol -= 1.0;
        horColour.g += 1.0 - xCol;
        horColour.b += xCol;
    }
    else {
        xCol -= 2.0;
        horColour.b += 1.0 - xCol;
        horColour.r += xCol;
    }

    //background lines
    float backValue = 1.0;
    float aspect = iResolution.x / iResolution.y;
    if (mod(uv.y * 100.0, 1.0) > 0.75 || mod(uv.x * 100.0 * aspect, 1.0) > 0.75) {
        backValue = 1.15;
    }

    vec3 backLines  = vec3(backValue);

    //main beam
    uv = (2.0 * uv) - 1.0;
    float beamWidth = abs(1.0 / (30.0 * uv.y));
    vec3 horBeam = vec3(beamWidth);

    gl_FragColor = vec4(((backLines * horBeam) * horColour), 0.1);
}
