#version 130

uniform sampler2D pack;
uniform vec2 tex_pos;
uniform vec2 tex_size;
uniform vec2 sprite_size;
uniform float func;
uniform float angle;


vec2 rotate_point(vec2 pt, float angle, vec2 or){
     return vec2(cos(angle) * (pt.x - or.x) - sin(angle) * (pt.y - or.y) + or.x,
				 sin(angle) * (pt.x - or.x) + cos(angle) * (pt.y - or.y) + or.y);
}

vec2 norm(vec2 p, vec2 lo, vec2 hi) 
{
	return (p - lo) / (hi - lo);
}

void main()
{
    // Enlargement per side, px
    float extra = abs(func) * max(sprite_size.x, sprite_size.y);

    // Calculate normalized texture coordinates, relative to a pack's texture.
    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;

    // Convert normalizaed texture coordinates to a zero to one range that is relative
    // to the size of the texture, not the pack.
    vec2 pack_size = textureSize(pack, 0);
	vec2 tex_lo_norm = tex_pos / pack_size;
	vec2 tex_hi_norm = (tex_pos + tex_size) / pack_size;
    vec2 tex_norm = norm(gl_TexCoord[0].xy, tex_lo_norm, tex_hi_norm);
    tex_norm = rotate_point(tex_norm, angle, vec2(0.5));

    // Find out the direction we need to go in order to add the enlargement to this side
    // In this code, we'll quite happily assume that the top-left and bottom-right uvs are (0,0) and (1,1) respectively
    // -1 + 2 * 0 = -1 => A value of 0 returns -1
    // -1 + 2 * 1 =  1 => A value of 1 returns 1
    vec2 offset = vec2(-1.0, -1.0) + 2.0 * tex_norm;

    // Now scale the current vertex to account for the enlargement
    vec4 vertex = gl_Vertex;
    vertex.x += offset.x * extra;
    vertex.y += offset.y * extra;

    // Shrink back the texture coordinates
    gl_TexCoord[0].xy = gl_TexCoord[0].xy + offset * extra / tex_norm;

    gl_Position = gl_ModelViewProjectionMatrix * vertex;
    gl_FrontColor = gl_Color;
}
