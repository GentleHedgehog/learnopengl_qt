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
        "out vec3 FragPosInWorld;\n"

        "uniform mat4 "+aMatrixHelper.model+";\n"
        "uniform mat4 "+aMatrixHelper.view+";\n"
        "uniform mat4 "+aMatrixHelper.projection+";\n"

        "void main(){\n"
        "gl_Position = " +
        aMatrixHelper.projection + "*" +
        aMatrixHelper.view + "*" +
        aMatrixHelper.model +
        " * vec4(" + aPos.name + ", 1.0);\n"

        "FragPosInWorld = vec3(" +
        aMatrixHelper.model +
        " * vec4(" + aPos.name + ", 1.0));\n"

        // calc with Normal matrix to consider changes in the model matrix for normals
        // (not efficient - calc on the CPU side):
//        "Normal = mat3(transpose(inverse(model))) * aNormal;\n"

        "Normal = aNormal;\n"
        "}";

QString fragmentShaderCode =
        "#version 330 core\n"

        "out vec4 fragColor;\n"
        "in vec3 Normal;\n"
        "in vec3 FragPosInWorld;\n"

        "uniform vec3 "+objectColor+";\n"
        "uniform vec3 "+lightColor+";\n"
        "uniform vec3 lightPos;\n"

        "void main(){\n"
        "vec3 norm = normalize(Normal);\n"
        "vec3 lightDir = normalize(lightPos - FragPosInWorld);\n"
        "float diff = max(dot(norm, lightDir), 0.0f);\n"
        "vec3 diffuse = diff * lightColor;\n"
        "float ambientKoef = 0.1f;\n"
        "vec3 ambient = lightColor * ambientKoef;\n"
        "fragColor = vec4(objectColor * (diffuse+ambient), 1.0f);\n"
        "}";

