#version 130

uniform vec2 screen_size;
uniform sampler2D target_tex;
uniform sampler2D light_tex;
uniform float norm_time;

void main()
{	
	vec2 pixel_xy = gl_FragCoord.xy / screen_size.xy;
    vec2 target_pixel_pos = vec2(pixel_xy.x, pixel_xy.y);
	vec4 target_color = texture(target_tex, target_pixel_pos);
	vec2 light_pixel_pos = vec2(norm_time, 0.0f);
	vec4 light_color = texture(light_tex, light_pixel_pos);

	gl_FragColor = mix(target_color, light_color, 1.0f);	
}
