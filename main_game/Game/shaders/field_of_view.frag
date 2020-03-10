#version 130

uniform vec2 screen_size;
uniform sampler2D tex;
uniform vec3 hero;

void main()
{	
	vec2 pixel_xy = gl_FragCoord.xy / screen_size.xy;
    vec2 pixel_pos = vec2(pixel_xy.x, pixel_xy.y);
	vec4 color = texture(tex, pixel_pos);
	
	float dist = distance(gl_FragCoord.xy, hero.xy);
	float r = hero.z * screen_size.x;

	float step1 = hero.z;
    float step2 = hero.z * 4;
	float att = dist / (r + 0.0001);

	vec4 dark = vec4(0.0, 0.0, 0.0, 0.75);
    color = mix(color, dark, smoothstep(step1, step2, att));

    gl_FragColor = color;
}