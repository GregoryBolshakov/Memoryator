#version 130

uniform vec2 screen_size;
uniform sampler2D tex;
//uniform sampler2D norm;
uniform vec3 light;

void main()
{	
	float dist = distance(gl_FragCoord.xy, light.xy);
	float r = light.z * screen_size.x;
	vec2 pixel_xy = gl_FragCoord.xy / screen_size.xy;
	//pixel_xy.y = 1.0 - pixel_xy.y;
    vec2 pixel_pos = vec2(pixel_xy.x, pixel_xy.y);
	vec4 color = texture(tex, pixel_pos);
    //vec3 normal_vector = normalize(texture(norm, pixel_pos).xyz * 2 - 1.0);
    //vec3 light_vector = normalize(vec3(vec2(light.xy - gl_FragCoord.xy), 10000.0));
    //float diffuse = max(dot(normal_vector, light_vector), 0);

    //gl_FragColor = mix(color, vec4(0.0, 0.0, 0.0, 1.0), dist / ((r * diffuse) + 0.0001));

	gl_FragColor = mix(color, vec4(0.0, 0.0, 0.0, 1.0), dist / (r + 0.0001));
	
}
