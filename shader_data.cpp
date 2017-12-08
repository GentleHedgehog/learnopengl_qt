#include "shader_data.h"

VA_HELPER_VERTEX_NORMAL(aPos, 0, aNormal, 1);


sMatrixHelper<QMatrix4x4> aMatrixHelper;

QString objectColor = "objectColor";
QString lightColor = "lightColor";

QString vertexShaderCode =
        "#version 330 core\n"

        "layout (location = "+
        aPos.location_str()+
        ") in vec3 " + aPos.name + ";\n"

        "layout (location = "+
        aNormal.location_str()+
        ") in vec3 " + aNormal.name + ";\n"

        "out vec3 Normal;\n"
        "out vec3 FragPos;\n"
        "out vec3 LightPos;\n"

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

        "FragPos = vec3(" +
        aMatrixHelper.view + "*" +
        aMatrixHelper.model +
        " * vec4(" + aPos.name + ", 1.0));\n"

        "LightPos = vec3(" +
        aMatrixHelper.view +
        " * vec4(lightPos, 1.0));\n"

        // calc with Normal matrix to consider changes in the model matrix for normals
        // (not efficient - calc on the CPU side):
        "Normal = mat3(transpose(inverse("+aMatrixHelper.view + "*" + aMatrixHelper.model+"))) * aNormal;\n"
//        "Normal = aNormal;\n"

        "}";

QString fragmentShaderCode =
        "#version 330 core\n"

        "struct Material{\n"
        "vec3 ambient;\n"
        "vec3 diffuse;\n"
        "vec3 specular;\n"
        "float shininess;\n"
        "};\n"
        "uniform Material material;\n"

        "in vec3 Normal;\n"
        "in vec3 FragPos;\n"
        "in vec3 LightPos;\n"

        "out vec4 fragColor;\n"

        "uniform vec3 "+objectColor+";\n"
        "uniform vec3 "+lightColor+";\n"
        "uniform vec3 cameraPos;\n"

        "void main(){\n"

        "float ambientKoef = 0.1f;\n"
        "vec3 ambient = lightColor * ambientKoef;\n"

        "vec3 norm = normalize(Normal);\n"
        "vec3 lightDir = normalize(LightPos - FragPos);\n"
        "float diff = max(dot(norm, lightDir), 0.0f);\n"
        "vec3 diffuse = diff * lightColor;\n"

        "float specularStrength = 0.5f;\n"
        // both a camera and a fragment position is in the world space:
        "vec3 viewDir = normalize(-FragPos);\n"//cameraPos = 0,0,0
        "vec3 reflectionDir = reflect(-lightDir, norm);\n"
        "int shininess = 32;\n"
        "float spec = pow(max(dot(viewDir, reflectionDir), 0.0f), shininess);\n"
        "vec3 specular = specularStrength * spec * lightColor;\n"

        "vec3 resultColor = vec3(objectColor * (diffuse+ambient+specular));\n"

        "fragColor = vec4(resultColor, 1.0f);\n"
        "}";

