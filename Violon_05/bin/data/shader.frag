uniform vec2 u_resolution; // 画面の解像度(width,height)
uniform float u_time; // 起動してからの経過時間(秒)
uniform vec2 u_point;

void main() {

    //画面の解像度から、0.0~1.0に正規化する
    vec2 st = gl_FragCoord.xy / u_resolution;
    // 色を格納する変数
    vec3 color;
    // RGBそれぞれの時間で変化するグラデーションを生成
    color.r = abs(sin(u_time * 10.3 + st.x));
    color.g = abs(sin(u_time * 20.3 + st.y));
    color.b = abs(sin(u_time * 5.0 + st.x));

    // 合成して出力
    gl_FragColor = vec4(color.r, color.g, color.b, 1.0);
    
//    float x = u_point.x;
//    float y = u_point.y;
//    float size = 100.0;
//    vec2  pos = vec2(x, y);
//    float dist = length(gl_FragCoord.xy - pos);
//    float color2 = size / dist;
//    gl_FragColor = vec4(color2/2.0, color2/2.0, color2/0.8, 1.0);
    
//    float xVal = (gl_FragCoord.x - u_point.x)/u_resolution.x;
//    float yVal = (gl_FragCoord.y - u_point.y)/u_resolution.y;
//    gl_FragColor = vec4(xVal, yVal, 0, 1);
    
//    float x = u_point.x;
//    float y = u_point.y;
//    float size = 100.0;
//    vec2  pos = vec2(x, y);
//    float dist = length(gl_FragCoord.xy - pos);
//    float color = size / dist;
//    gl_FragColor = vec4(color/2.0, color/2.0, color/0.8, 1.0);

}