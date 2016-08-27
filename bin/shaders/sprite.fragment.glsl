#version 330

uniform sampler2D sampler0;
uniform mat4 uvmodel;

in vec4 fg_color;
in vec2 fg_uv;

out vec4 final_color;

void main(void) {
	vec4 position = uvmodel * vec4(fg_uv, 0.0, 1.0);
	vec4 image_color = texture(sampler0, vec2(position.xy));
	final_color = fg_color * image_color;
}

