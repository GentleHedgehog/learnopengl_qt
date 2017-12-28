#include "shader_data.h"

VA_HELPER_VERTEX_NORMAL_TEXTURE(aPos, 0, aNormal, 1, aTexture, 2);


sMatrixHelper<QMatrix4x4> aMatrixHelper;

QString vertexShaderCode =
        "#version 330 core\n"

        "layout (location = "+
        aPos.location_str()+
        ") in vec3 " + aPos.name + ";\n"

        "layout (location = "+
        aNormal.location_str()+
        ") in vec3 " + aNormal.name + ";\n"

        "layout (location = "+
        aTexture.location_str()+
        ") in vec2 " + aTexture.name + ";\n"

        "out vec3 Normal;\n"
        "out vec3 FragPos;\n"
        "out vec3 LightPos;\n"
        "out vec2 TexCoords;\n"

        "uniform vec3 lightPos;\n"
        "uniform mat4 "+aMatrixHelper.model+";\n"
        "uniform mat4 "+aMatrixHelper.view+";\n"
        "uniform mat4 "+aMatrixHelper.projection+";\n"

        "void main(){\n"
            "gl_Position = " +
            aMatrixHelper.projection + "*" +
            aMatrixHelper.view + "*" +
            aMatrixHelper.model +
            " * vec4(" + aPos.name + ", 1.0);\n"

            //frag pos in certain space:
            "FragPos = vec3(" +
            aMatrixHelper.view + "*" +
            aMatrixHelper.model +
            " * vec4(" + aPos.name + ", 1.0));\n"

            "LightPos = vec3(" +
            aMatrixHelper.view +
            " * vec4(lightPos, 1.0));\n"

            "TexCoords = aTexture;\n"

            // calc with Normal matrix to consider changes in the model matrix for normals
            // (not efficient - calc on the CPU side):
            "Normal = mat3(transpose(inverse("+aMatrixHelper.view + "*" + aMatrixHelper.model+"))) * aNormal;\n"
    //        "Normal = aNormal;\n"

        "}";

QString fragmentShaderCode =
        "#version 330 core\n"

        "struct Material{\n"
            "sampler2D diffuse;\n"
            "sampler2D specular;\n"
            "float shininess;\n"
        "};\n"
        "uniform Material material;\n"


        "struct Light{\n"

            "vec3 position;\n" // camera position (if in world coords?)

            "vec3 ambient;\n"
            "vec3 diffuse;\n"
            "vec3 specular;\n"

            "float constant;\n"
            "float linear;\n"
            "float quadratic;\n"

            "vec3 spotlightDirection;\n"
            "float spotlightOuterCutOff;\n"
            "float spotlightInnerCutOff;\n"

        "};\n"
        "uniform Light light;\n"

        "in vec3 Normal;\n"
        "in vec3 FragPos;\n"
        "in vec3 LightPos;\n"
        "in vec2 TexCoords;\n"

        "out vec4 fragColor;\n"

        "uniform vec3 cameraPos;\n"

        "void main(){\n"

            "vec3 resultColor;\n"
            "vec3 ambient = light.ambient * "
                            "vec3(texture(material.diffuse, "
                            "TexCoords));\n"


            "vec3 lightDir = normalize(light.position - FragPos);\n"
            // angle between spotDir and lightDir:
            "float theta = dot(lightDir, "
            "normalize(-light.spotlightDirection));\n"

            "float epsilon = light.spotlightInnerCutOff - light.spotlightOuterCutOff;\n"
            "float intensity = clamp((theta - light.spotlightOuterCutOff)/epsilon, 0.0, 1.0);\n"


            "vec3 norm = normalize(Normal);\n"
//                "vec3 lightDir = normalize(LightPos - FragPos);\n"
            "float diff = max(dot(norm, lightDir), 0.0f);\n"
            "vec3 diffuse = diff * vec3(texture(material.diffuse, TexCoords)) * light.diffuse;\n"

            "vec3 viewDir = normalize(-FragPos);\n"//cameraPos = 0,0,0
            "vec3 reflectionDir = reflect(-lightDir, norm);\n"
            "float shininess = material.shininess;\n"
            "float spec = pow(max(dot(viewDir, reflectionDir), 0.0f), shininess);\n"
            "vec3 specular = "
            "(spec * vec3(texture(material.specular, TexCoords))) *"
            " light.specular;\n"

            "float distance = length(light.position - FragPos);\n"
            "float attenuation = "
            "1.0 / ("
            "light.constant + light.linear * distance +"
            "light.quadratic * (distance * distance)"
            ");\n"
//            "ambient *= attenuation;\n"
//            "diffuse *= attenuation;\n"
//            "specular *= attenuation;\n"

            "diffuse *= intensity;\n"
            "specular *= intensity;\n"

            "resultColor = diffuse+ambient+specular;\n"



            "fragColor = vec4(resultColor, 1.0f);\n"
        "}";

