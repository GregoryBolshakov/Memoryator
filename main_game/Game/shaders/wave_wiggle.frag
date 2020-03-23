#version 130

uniform sampler2D pack;
uniform vec2 tex_pos;
uniform vec2 tex_size;
uniform vec2 sprite_pos;
uniform vec2 sprite_size;
uniform vec2 screen_size;
uniform float func;
uniform float angle;


bool in_box(vec2 p, vec2 lo, vec2 hi) {
    return p.x >= lo.x && p.y >= lo.y && p.x <= hi.x && p.y <= hi.y;
}

vec2 norm(vec2 p, vec2 lo, vec2 hi) 
{
	return (p - lo) / (hi - lo);
}

vec2 rotate_point(vec2 pt, float angle, vec2 or){

     return vec2(cos(angle) * (pt.x - or.x) - sin(angle) * (pt.y - or.y) + or.x,
				 sin(angle) * (pt.x - or.x) + cos(angle) * (pt.y - or.y) + or.y);
}

void main()
{
	vec2 pack_size = textureSize(pack, 0);
	vec2 tex_pos_norm = tex_pos / pack_size;
	vec2 tex_size_norm = tex_size / pack_size;

	vec2 frag_coord = vec2(gl_FragCoord.x, screen_size.y - gl_FragCoord.y);
	vec2 lo = vec2(sprite_pos.x, sprite_pos.y);
	vec2 hi = vec2(sprite_pos.x + sprite_size.x, sprite_pos.y + sprite_size.y);
	
	vec2 coord_norm = norm(frag_coord, lo, hi);		

	float y = coord_norm.y;
	float fy = 1.0 - y * (2.0 - y);
	float f =  func * fy;

	vec2 axis = vec2(1.0, 0.0);
	axis = rotate_point(axis, -angle, vec2(0.0));

	coord_norm = rotate_point(coord_norm, -angle, vec2(0.5));
	coord_norm +=  f * axis;

	vec2 tex_coord_norm = tex_pos_norm + ((tex_pos_norm + tex_size_norm) - tex_pos_norm) * coord_norm;

	vec4 color = texture(pack, tex_coord_norm);
	color.a *= int(in_box(coord_norm, vec2(0), vec2(1.0)));

//	vec2 p = 1.0 / sprite_size;	
//	if(coord_norm.x > (0.1 - p.x / 2.0 ) && coord_norm.x < (0.1 + p.x / 2.0) || coord_norm.y > (0.1 - p.y / 2.0 ) && coord_norm.y < (0.1 + p.y / 2.0))
//	{
//		color = uvec4(0.0, 0.0, 1.0, 1.0);
//	}
		
    gl_FragColor = color;	
}