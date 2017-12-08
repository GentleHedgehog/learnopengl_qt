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
    aNormal.applyAttrib(f);

    VAO.release();
}
#include <QtMath>
void Lighting::doPaintWork()
{
    static float counter = 0.0f;
    counter += 0.009f;

    float x = 0.0f, z = 0.0f, y = 0.0f;
    x = qCos(qDegreesToRadians((float)((int)counter % 360)));
    z = qSin(qDegreesToRadians((float)((int)counter % 360)));
    y = qCos(qDegreesToRadians((float)((int)counter % 360)));

    lightPos.setX(0.5);
    lightPos.setY(y);
    lightPos.setZ(qAbs(z));

    aMatrixHelper.modelMat.translate(lightPos);
    aMatrixHelper.modelMat.scale(0.2f);
    aMatrixHelper.loadMatrixToShader(lightingProgram);
    aMatrixHelper.modelMat.translate(lightPos * -1);

    lightingProgram->use();
    VAO.bind();
    glDrawElements(GL_TRIANGLES, 6*6, GL_UNSIGNED_INT, 0);
    VAO.release();

    programSet->use();
    programSet->setUniformValue("material.ambient",
                                QVector3D(1.0f, 0.5f, 0.31f));
    programSet->setUniformValue("material.diffuse",
                                QVector3D(1.0f, 0.5f, 0.31f));
    programSet->setUniformValue("material.specular",
                                QVector3D(0.5f, 0.5f, 0.5f));
    programSet->setUniformValue("material.shininess",
                                32.0f);
}
