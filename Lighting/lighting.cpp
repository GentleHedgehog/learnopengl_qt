#include "lighting.h"
#include "shader_data_for_lighting.h"

using namespace nLighting;

Lighting::Lighting(QObject *parent) :
    AccessToQtOpenGl(parent)
{

}

void Lighting::initVAO(int VBO)
{
    VAO.create();
    VAO.bind();

    f->glBindBuffer(GL_ARRAY_BUFFER, VBO);

    f->glVertexAttribPointer(SHD_LOCATION_A_POS,
                             SHD_ELEMENT_COUNT_A_POS,
                             GL_FLOAT, GL_FALSE,
                             SHD_STRIDE_A_POS,
                             (void*)SHD_OFFSET_A_POS);
    f->glEnableVertexAttribArray(SHD_LOCATION_A_POS);

    VAO.release();
}

void Lighting::doPaintWork(int VBO)
{
    VAO.bind();
    programSet->use();
    programSet->setUniformValue(SHD_OBJECT_COLOR_NAME.toUtf8().constData(),
                                QVector3D(1.0f, 0.5f, 0.31f));
    programSet->setUniformValue(SHD_LIGHT_COLOR_NAME.toUtf8().constData(),
                                QVector3D(1.0f, 1.0f, 1.0f));
    VAO.release();
}
