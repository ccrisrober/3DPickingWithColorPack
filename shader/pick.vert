#version 430
layout (location = 0) in vec3 Position;

uniform mat4 modelViewProj;

void main() {
    gl_Position = modelViewProj * vec4(Position,1.0);
}
