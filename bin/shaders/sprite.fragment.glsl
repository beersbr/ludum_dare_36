#version 330

uniform sampler2D sampler0;
uniform mat4 uvmodel;

in vec4 fg_color;
in vec2 fg_uv;

out vec4 final_color;

void main(void) {
	final_color = fg_color * texture(sampler0, vec2(uvmodel * vec4(fg_uv, 0.0, 1.0)));
}

