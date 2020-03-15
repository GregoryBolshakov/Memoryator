#version 130

#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D scene_tex;
uniform sampler2D overlay_tex;
uniform sampler2D multiply_tex;
uniform sampler2D fov_tex;
uniform vec2 scene_size;
uniform vec3 view;
uniform float norm_time;

float overlay_blend(in float back, in float front) 
{
	return (1.0 - step(0.5, back)) * ( 2.0 * back * front) + step(0.5, back) * (1.0 - 2.0 * (1.0 - back) * (1.0 - front));
}

float opacity(in float back, in float front, in float op)
{
	return op * front + (1 - op) * back;
}

float circle(in vec2 st, in vec2 center, in float radius)
{
    vec2 dist = st - center;
	return 1.0 - smoothstep(radius - (radius * 1.2), radius + (radius * 0.5), dot(dist, dist) * 1.0);
}

void main()
{	
	vec2 pixel_xy = gl_FragCoord.xy / scene_size.xy;
	vec4 scene_color = texture(scene_tex, pixel_xy);

	vec2 timed_pixel_pos = vec2(norm_time, 0.0f);
	vec4 overlay_color = texture(overlay_tex, timed_pixel_pos);
	vec4 multiply_color = texture(multiply_tex, timed_pixel_pos);
	vec4 fov_color = texture(fov_tex, timed_pixel_pos);

	vec4 result_color = vec4(1.0);

	// color correction

	result_color.r = overlay_blend(scene_color.r, overlay_color.r);
	result_color.g = overlay_blend(scene_color.g, overlay_color.g);
	result_color.b = overlay_blend(scene_color.b, overlay_color.b);

	result_color.r = opacity(scene_color.r, result_color.r, overlay_color.a);
	result_color.g = opacity(scene_color.g, result_color.g, overlay_color.a);
	result_color.b = opacity(scene_color.b, result_color.b, overlay_color.a);

	// part of the day
	
	vec3 mult  = result_color.rgb * multiply_color.rgb;

	result_color.r = opacity(result_color.r, mult.r, multiply_color.a);
	result_color.g = opacity(result_color.g, mult.g, multiply_color.a);
	result_color.b = opacity(result_color.b, mult.b, multiply_color.a);

	// field of view

	float fov = 0.25 * (1.0 + view.z) / (fov_color.a + 0.00001);
	float op = 1.0 - circle(pixel_xy, view.xy, fov);
	mult = result_color.rgb * fov_color.rgb;

	result_color.r = opacity(result_color.r, mult.r, op);
	result_color.g = opacity(result_color.g, mult.g, op);
	result_color.b = opacity(result_color.b, mult.b, op);

	gl_FragColor = result_color;
}
