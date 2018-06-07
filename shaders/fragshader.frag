#version 330 core

// Interpolated values from the vertex shaders
in vec3 Position_worldspace;
in vec3 Position_cameraspace;
in vec3 Normal_cameraspace;
in vec3 EyeDirection_cameraspace;
in vec3 LightDirection_cameraspace;
in vec3 colour_ambient;
in vec3 colour_diffuse;
in vec3 colour_specular;

// Ouput data
out vec3 color;

// Values that stay constant for the whole mesh.
uniform sampler2D myTextureSampler;
uniform mat4 MV;
uniform vec3 LightPosition_worldspace;
uniform vec3 LightColor;
uniform float LightPower;
uniform float SpecularPower;
uniform float Smoothness;
uniform float AmbientPower;

void main(){  
        //normal vector in cameraspace from interpolated position using the partial derivatives method
        vec3 Normal_cameraspace = normalize(cross(dFdx(Position_cameraspace), dFdy(Position_cameraspace)));

        // Distance to the light
        float distance = length( LightPosition_worldspace - Position_worldspace );

        // Normal of the computed fragment, in camera space
        vec3 n = normalize( Normal_cameraspace );
        // Direction of the light (from the fragment to the light)
        vec3 l = normalize( LightDirection_cameraspace );
        // Cosine of the angle between the normal and the light direction,
        // clamped above 0
        //  - light is at the vertical of the triangle -> 1
        //  - light is perpendicular to the triangle -> 0
        //  - light is behind the triangle -> 0
        float cosTheta = clamp( dot( n,l ), 0,1 );

        // Eye vector (towards the camera)
        vec3 E = normalize(EyeDirection_cameraspace);
        // Direction in which the triangle reflects the light
        vec3 R = reflect(-l,n);
        // Cosine of the angle between the Eye vector and the Reflect vector,
        // clamped to 0
        //  - Looking into the reflection -> 1
        //  - Looking elsewhere -> < 1
        float cosAlpha = clamp( dot( E,R ), 0,1 );

        float cosBeta = dot(n,E);

        // Ambient : simulates indirect lighting
        vec3 AmbientColor = AmbientPower*colour_ambient;
         // Diffuse : "color" of the object
        vec3 DiffuseColor = colour_diffuse * LightColor * LightPower * cosTheta / (distance*distance);
        //vec3 DiffuseColor = 0.5*clamp(colour_diffuse * LightColor * LightPower * cosTheta / (distance*distance),vec3(0.0, 0.0, 0.0),colour_diffuse);
        // Specular : reflective highlight, like a mirror
        vec3 SpecularColor = colour_specular * LightColor * LightPower * SpecularPower * pow(cosAlpha,Smoothness) / (distance*distance);


        if(cosBeta < 0){
            DiffuseColor = vec3(0,0,0);
        }
        color =  AmbientColor+DiffuseColor + SpecularColor;
}
