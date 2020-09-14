#type vertex

#version 430

layout(location=0) in vec3 pos;
layout(location=1) in vec4 color;
layout(location=2) in vec2 uv;
layout(location=3) in vec3 normal;

// uniform location 0 global for program.
layout(location=0) uniform mat4 projection;
layout(location=1) uniform mat4 view;
layout(location=2) uniform mat4 model;

layout(location=0) out vec4 Color;
layout(location=1) out vec2 UV;
layout(location=2) out vec3 Normal;
void main()
{
	gl_Position = model * vec4(pos, 1);
	Color = color;
	UV = uv;
	Normal = normal;
}


#type fragment

#version 430
layout(location=0) in vec4 color;
layout(location=1) in vec2 uv;
layout(location=2) in vec3 position;

// uniform location 1 global for program
layout(location=3) uniform sampler2D wakeMeUpInside;

out vec4 CantWakeUp;
void main()
{
	CantWakeUp = vec4(1,1,1,1);
}