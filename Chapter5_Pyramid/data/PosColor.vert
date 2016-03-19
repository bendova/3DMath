#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;

out vec4 fragColor;

uniform mat4 modelToCameraTransform;
uniform mat4 cameraToClipTransform;

void main()
{
	fragColor = color;
	gl_Position = cameraToClipTransform * modelToCameraTransform * vec4(position, 1.0);

	// The code below seems to fix the z-fighting problems,
	// but how the FUCK does it do it?
	// It also, generates some intersesting visual artifacts
	// for objects that are near the camera.
	float C = 1.0;
	float far = 1000.0;
	float offset = 1.0;
	gl_Position.z = 2 * log(C * gl_Position.w + offset) / log(C * far + offset) - offset;
	gl_Position.z *= gl_Position.w;
}