#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 color_ambient;
layout(location = 2) in vec3 color_diffuse;
layout(location = 3) in vec3 color_specular;

out vec3 Position_worldspace;
out vec3 Position_cameraspace;
out vec3 EyeDirection_cameraspace;
out vec3 LightDirection_cameraspace;
out vec3 colour_ambient;
out vec3 colour_diffuse;
out vec3 colour_specular;

// Values that stay constant for the whole mesh.
uniform mat4 MVP;
uniform mat4 V;
uniform mat4 M;
uniform vec3 LightPosition_worldspace;
uniform vec3 LightColor;
uniform float LightPower;
uniform float SpecularPower;
uniform float Smoothness;

void main(){

        // Output position of the vertex, in clip space : MVP * position
        gl_Position =  MVP * vec4(vertexPosition_modelspace,1);

        // Position of the vertex, in worldspace : M * position
        Position_worldspace = (M * vec4(vertexPosition_modelspace,1)).xyz;

        // Position of the vertex, in cameraspace : M * V * position
        Position_cameraspace = (M * V * vec4(vertexPosition_modelspace,1)).xyz;

        // Vector that goes from the vertex to the camera, in camera space.
        // In camera space, the camera is at the origin (0,0,0).
        vec3 vertexPosition_cameraspace = ( V * M * vec4(vertexPosition_modelspace,1)).xyz;
        EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;

        // Vector that goes from the vertex to the light, in camera space. M is ommited because it's identity.
        vec3 LightPosition_cameraspace = ( V * vec4(LightPosition_worldspace,1)).xyz;
        LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;

        colour_ambient=color_ambient;
        colour_diffuse=color_diffuse;
        colour_specular=color_specular;
}

