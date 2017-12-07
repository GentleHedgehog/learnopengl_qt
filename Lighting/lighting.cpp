#include "lighting.h"
#include "shader_data.h"
#include <cassert>

namespace {

ShaderProgramSet *lightingProgram;

QString fragmentShaderCodeForLight =
        "#version 330 core\n"

        "out vec4 fragColor;\n\n"

        "void main(){\n"
        "fragColor = vec4(1.0f);\n"
        "}";

QVector3D lightPos(1.2f, 1.0f, 2.0f);

}

Lighting::Lighting(QObject *parent) :
    AccessToQtOpenGl(parent)
{
}

void Lighting::initVAO(QOpenGLBuffer vbo, QOpenGLBuffer ebo)
{
    lightingProgram = new ShaderProgramSet(
                vertexShaderCode, fragmentShaderCodeForLight,
                context, f, this);
    lightingProgram->compile();
    assert(lightingProgram->link());


    assert(VAO.create());
    VAO.bind();

    assert(vbo.bind());
    assert(ebo.bind());

    aPos.applyAttrib(f);
    aColor.applyAttrib(f);
    aTextureCoord.applyAttrib(f);

    VAO.release();
}

void Lighting::doPaintWork()
{

    aMatrixHelper.modelMat.translate(lightPos);
    aMatrixHelper.modelMat.scale(0.2f);
    aMatrixHelper.loadMatrixToShader(lightingProgram);
    aMatrixHelper.modelMat.translate(lightPos * -1);

    VAO.bind();
    glDrawElements(GL_TRIANGLES, 6*6, GL_UNSIGNED_INT, 0);
    VAO.release();
}
