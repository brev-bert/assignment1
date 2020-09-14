#version 430


layout(location=0) in vec3 normalInterp;
layout(location=1) in vec3 position;
layout(location=2) in vec2 uv;
// Shader lifted from https://en.wikipedia.org/wiki/Blinn%E2%80%93Phong_reflection_model

layout(location=10) uniform sampler2D diffuseTexture;
struct Light
{
    vec3 Pos;
    vec3 Color;
    float Power;
    vec3 Ambi;
    vec3 Spec;
};
//layout(location=11) uniform int lightNumber;
layout(location=12) uniform int mode;
layout(location=13) uniform Light light;

// Should normally be uniform per object.
const float shininess = 32.0;
const float screenGamma = 2.2; //srgb

layout(location=0) out vec4 Out;

void main()
{
    // Used for diffuse and alpha value.
    vec4 tex = texture(diffuseTexture, uv, 0);

    // Same for each light.
    vec3 normal = normalize(normalInterp);

    // "Light bucket" for this fragment.
    vec3 colorLinear = vec3(0.0);

    // for light in lights
    vec3 lightDir = light.Pos - position;
    float distance = length(lightDir);
    distance = distance * distance;
    lightDir = normalize(lightDir);

    float lambertian = max(dot(lightDir, normal), 0.0);
    float specular = 0.0;

    if (lambertian > 0.0) {
        vec3 viewDir = normalize(-position);

        // Blinn phong
        vec3 halfDir = normalize(lightDir + viewDir);
        float specAngle = max(dot(halfDir, normal), 0.0);
        specular = pow(specAngle, shininess);

        // phong only?
        if (mode == 2) {
            vec3 reflectDir = reflect(-lightDir, normal);
            specAngle = max(dot(reflectDir, viewDir), 0.0);
            specular = pow(specAngle, shininess/4.0);
        }
            
        colorLinear +=  light.Ambi +
                        tex.rgb * lambertian * light.Color * light.Power / distance +
                        light.Spec * specular * light.Color * light.Power / distance;
    }
    // end for light in lights

    // Gamma correct color (assume it wasn't already)
    float gu = 1.0 / screenGamma;
    vec3 colorGammaCorrected = pow(colorLinear, vec3(gu, gu, gu));

	Out = vec4(colorGammaCorrected, tex.a);
}