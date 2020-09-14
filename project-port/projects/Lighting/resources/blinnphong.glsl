#type vertex

#version 430
layout(location=0) in vec3 pos;
layout(location=2) in vec2 uv;
layout(location=3) in vec3 normal;

// uniform location 0 global for program.
layout(location=0) uniform mat4 projection;
layout(location=1) uniform mat4 modelView;
layout(location=2) uniform mat4 normalMat;

layout(location=0) out vec3 NormalInterp;
layout(location=1) out vec3 Pos;
layout(location=2) out vec2 UV;

void main()
{
	gl_Position = projection * modelView * vec4(pos, 1);
    vec4 vertPos4 = modelView * vec4(pos, 1.0);
    Pos = vec3(vertPos4) / vertPos4.w;
	UV = uv;
    //normalMat = transpose(inverse(model)) * normal 
	//NormalInterp = vec3(normalMat * vec4(normal, 0.0));
    NormalInterp = mat3(normalMat) * normal;
}
#type fragment

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
const float shininess = 16.0;
const float screenGamma = 2.2; //srgb

layout(location=0) out vec4 Out;

void main()
{
    // Used for diffuse and alpha value.
    vec4 tex = texture(diffuseTexture, uv, 0);

    // Same for each light.
    vec3 normal = normalize(normalInterp);

    // "Light bucket" for this fragment (if multiple lights).
    //vec3 colorLinear = vec3(0.0);

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
        /*if (mode == 2) {
            vec3 reflectDir = reflect(-lightDir, normal);
            specAngle = max(dot(reflectDir, viewDir), 0.0);
            specular = pow(specAngle, shininess/4.0);
        }*/
            
        // Add to light bucket if multiple lights.
        /*colorLinear += light.Ambi * tex.xyz +
                       (tex.xyz * lambertian * light.Color * light.Power) / distance +
                       (light.Spec * specular * light.Color * light.Power) / distance*/;
    }
    // end for light in lights

    vec3 colorLinear =  light.Ambi * tex.rgb +
        (tex.rgb * lambertian * light.Color * light.Power) / distance +
        (light.Spec * specular * light.Color * light.Power) / distance;

    // Gamma correct color (assume it wasn't already)
    float gu = 1.0 / screenGamma;
    vec3 colorGammaCorrected = colorLinear;//= pow(colorLinear, vec3(gu, gu, gu));

	Out = vec4(colorGammaCorrected, tex.a);
    //Out = tex;
}