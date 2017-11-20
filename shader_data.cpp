#include "shader_data.h"


QString vertexShaderCode =
        "#version 330 core\n"
        "layout (location = "+
        QString::number(SHD_LOCATION_A_POS)+
        ") in vec3 aPos;\n"
        "layout (location = "+
        QString::number(SHD_LOCATION_A_COLOR)+
        ") in vec3 aColor;\n"
        "layout (location = "+
        QString::number(SHD_LOCATION_A_TEX_COORD)+
        ") in vec2 aTexCoord;\n"
        "out vec2 texCoord;\n"
        "out vec4 usualColor;\n"
        "uniform mat4 "+SHD_MODEL_MATRIX_NAME+";\n"
        "uniform mat4 "+SHD_VIEW_MATRIX_NAME+";\n"
        "uniform mat4 "+SHD_PROJ_MATRIX_NAME+";\n"
        "void main(){\n"
        "gl_Position = " +
        SHD_PROJ_MATRIX_NAME + "*" +
        SHD_VIEW_MATRIX_NAME + "*" +
        SHD_MODEL_MATRIX_NAME +
        " * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "usualColor = vec4(aColor, 1.0);\n"
        "texCoord = aTexCoord;\n"
        "}";

ShaderProgramSet programUsual(
        vertexShaderCode
        ,

        "#version 330 core\n"
        "in vec4 usualColor;\n"
        "in vec2 texCoord;\n"
        "out vec4 fragColor;\n\n"
        "uniform float mixValue;\n"
        "uniform sampler2D ourTexture1;\n"
        "uniform sampler2D ourTexture2;\n\n"
        "void main(){\n"
//            "fragColor = usualColor;\n"
        "vec4 color1 = texture(ourTexture1, texCoord);\n"
//            "vec4 color1 = vec4(1.0, 1.0, 1.0, 1.0);\n"
        "vec4 color2 = texture(ourTexture2, vec2(texCoord.x,texCoord.y));\n"
//            "vec4 color2 = vec4(1.0, 0.0, 0.0, 1.0);\n"
        "fragColor = mix("
        "color1.rgba,"
        "color2.rgba, "
        "mixValue);\n"
        "}"
        );

ShaderProgramSet programYellow(
        vertexShaderCode
        ,

        "#version 330 core\n"
        "uniform vec4 uniformColor;\n"
        "out vec4 color;\n\n"
        "void main(){\n"
        "color = uniformColor;\n"
        "}"
        );
