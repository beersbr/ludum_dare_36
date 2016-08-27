#version 330

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

layout(location=0) in vec3 position;
layout(location=1) in vec4 color;
layout(location=2) in vec2 uv;

out vec4 fg_color;
out vec2 fg_uv;

void main(void) {
	fg_color = color;
	fg_uv    = uv;

	gl_Position = projection * view * model * vec4(position, 1.0);

}

