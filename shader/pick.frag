#version 430
//layout (early_fragment_tests) in;

out vec4 ourColor;

uniform int id;

vec3 unpackColor3(float f) {
    vec3 color;
    color.r = floor(f / 256.0 / 256.0);
    color.g = floor((f - color.r * 256.0 * 256.0) / 256.0);
    color.b = floor(f - color.r * 256.0 * 256.0 - color.g * 256.0);
    // now we have a vec3 with the 3 components in range [0..256]. Let's normalize it!
    return color / 255.0;
}

uniform bool selected;

void main() {
	ourColor = vec4(unpackColor3(float(id)), 1.0);
	if(selected) {
		ourColor = vec4(vec3(1.0, 0.0, 0.0), 1.0);
	}
}