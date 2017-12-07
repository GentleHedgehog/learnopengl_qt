#include "shader_data.h"

VBO_HELPER_VERTEX_COLOR_TEXTURE(aPos, 0, aColor, 1, aTextureCoord, 2);

sMatrixHelper<QMatrix4x4> aMatrixHelper;

QString objectColor = "objectColor";
QString lightColor = "lightColor";

QString vertexShaderCode =
        "#version 330 core\n"

        "layout (location = "+
        aPos.location_str()+
        ") in vec3 " + aPos.name + ";\n"

        "layout (location = "+
        aColor.location_str()+
        ") in vec3 " + aColor.name + ";\n"

        "layout (location = "+
        aTextureCoord.location_str()+
        ") in vec2 " + aTextureCoord.name + ";\n"

        "uniform mat4 "+aMatrixHelper.model+";\n"
        "uniform mat4 "+aMatrixHelper.view+";\n"
        "uniform mat4 "+aMatrixHelper.projection+";\n"

        "void main(){\n"
        "gl_Position = " +
        aMatrixHelper.projection + "*" +
        aMatrixHelper.view + "*" +
        aMatrixHelper.model +
        " * vec4(" + aPos.name + ", 1.0);\n"
        "}";

QString fragmentShaderCode =
        "#version 330 core\n"

        "out vec4 fragColor;\n\n"

        "uniform vec3 "+objectColor+";\n"
        "uniform vec3 "+lightColor+";\n"

        "void main(){\n"
//        "fragColor = vec4("+objectColor+" * "+lightColor+", 1.0f);\n"
          "fragColor = vec4(0.0f, 1.0f, 0.0, 1.0f);\n"
        "}";

