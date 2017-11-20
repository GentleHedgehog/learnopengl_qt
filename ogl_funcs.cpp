#include "ogl_funcs.h"
#include "UsableClass/Macros/macros.h"
#include <QOpenGLVertexArrayObject>
#include <QImage>

#include "vertex_data.h"
#include "shader_data.h"
#include "camera_setter.h"
#include "texture_holder.h"

typedef OGL_funcs cls;

namespace
{
    QOpenGLVertexArrayObject VAO[2];
    quint32 EBO[2] = {0};
    quint32 VBO[2] = {0};

    bool isPolygoneModeLine = false;
    bool isPolygoneModeFill = true;
    bool isChangePolygoneMode = false;

    CameraSetter aCameraSetter;
    TextureHolder aTextureHolder;
}

cls::OGL_funcs(QWidget *parent) :
    QGLWidget(parent)
{
    resize(aCameraSetter.screenWidth, aCameraSetter.screenHeight);

    connect(&timer, SIGNAL(timeout()),
            this, SLOT(updateGL()) );
    timer.start(10);
}

void cls::createBufObjectsForVertices(bool isFirstTriangle)
{
    if (isFirstTriangle)
    {
        quint32 bufCountForGen = 2;
        glGenBuffers(bufCountForGen, VBO);
        glGenBuffers(bufCountForGen, EBO);

//        DEBUG_NM(VBO[0]);
//        DEBUG_NM(EBO[0]);
//        DEBUG_NM(VBO[1]);
//        DEBUG_NM(EBO[1]);

        glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_first_cube),
                     static_cast<void*>(vertices_first_cube), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_first_cube),
                     static_cast<void*>(indices_first_cube), GL_STATIC_DRAW);
    }
    else
    {
        glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_second_triangle),
                     static_cast<void*>(vertices_second_triangle), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_second_triangle),
                     static_cast<void*>(indices_second_triangle), GL_STATIC_DRAW);
    }

}

void cls::setAttribFroVertexAPos()
{
    glVertexAttribPointer(SHD_LOCATION_A_POS,
                          SHD_ELEMENT_COUNT_A_POS,
                          GL_FLOAT, GL_FALSE,
                          SHD_STRIDE_A_POS,
                          (void*)SHD_OFFSET_A_POS);
    glEnableVertexAttribArray(SHD_LOCATION_A_POS);

    glVertexAttribPointer(SHD_LOCATION_A_COLOR,
                          SHD_ELEMENT_COUNT_A_COLOR,
                          GL_FLOAT, GL_FALSE,
                          SHD_STRIDE_A_COLOR,
                          (void*)SHD_OFFSET_A_COLOR);
    glEnableVertexAttribArray(SHD_LOCATION_A_COLOR);

    glVertexAttribPointer(SHD_LOCATION_A_TEX_COORD,
                          SHD_ELEMENT_COUNT_A_TEX_COORD,
                          GL_FLOAT, GL_FALSE,
                          SHD_STRIDE_A_TEX_COORD,
                          (void*)SHD_OFFSET_A_TEX_COORD);
    glEnableVertexAttribArray(SHD_LOCATION_A_TEX_COORD);
}

void cls::initializeGL()
{
//    DEBUG(__PRETTY_FUNCTION__);

    initializeGLFunctions(QGLContext::currentContext());

    glClearColor(1.0, 1.0, 1.0, 1.0);

//    qint32 nMaxVertexAttribs = 0;
//    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nMaxVertexAttribs);
//    DEBUG_NM(nMaxVertexAttribs);

//    DEBUG_NM(QGLContext::currentContext()->
//             format().majorVersion());
//    DEBUG_NM(QGLContext::currentContext()->
//             format().minorVersion());


    assert(VAO[0].create());
    VAO[0].bind();
    createBufObjectsForVertices(true);
    setAttribFroVertexAPos();
    VAO[0].release();


//    assert(VAO[1].create());
//    VAO[1].bind();
//    createBufObjectsForVertices(false);
//    setAttribFroVertexAPos();
//    VAO[1].release();

    programUsual.initialize(
                QGLContext::currentContext(),
                this);
    programUsual.compile();
    programUsual.link();

//    createShaders(programYellow);
//    createProgramWithShaders(programYellow);
//    deleteShaders(programYellow);

    aTextureHolder.initialize(QGLContext::currentContext(),
                              this, &programUsual);

    aCameraSetter.initialize(QGLContext::currentContext(),
                              this, &programUsual);


}

void cls::paintGL()
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);

    programUsual.useProgram();

//    DEBUG("repaint");

    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    aTextureHolder.doPaintWork();

    VAO[0].bind();
//    glDrawArrays(GL_TRIANGLES, 0, 3);

    for (int i = 0; i < sizeofArray(cubesPositions); ++i)
    {
        QVector3D vecForModelTranslation = cubesPositions[i];
        QVector3D vecForModelRotation = QVector3D(0.0, 1.0, 0.0f);

        float rotateAngle = 1;//(float)(QTime::currentTime().msec() % 5);

//        cubesModelMatrices[i].setToIdentity();
        cubesModelMatrices[i].translate(vecForModelTranslation);
        cubesModelMatrices[i].rotate(rotateAngle, vecForModelRotation);

        programUsual.setUniformMatrixValue(SHD_MODEL_MATRIX_NAME,
                                           cubesModelMatrices[i].constData());
        cubesModelMatrices[i].translate(vecForModelTranslation * -1);

        glDrawElements(GL_TRIANGLES, 6*6, GL_UNSIGNED_INT, 0);
    }

    VAO[0].release();

    if (isChangePolygoneMode)
    {
        if (isPolygoneModeLine)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        if (isPolygoneModeFill)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        isChangePolygoneMode = false;
        isPolygoneModeLine = false;
        isPolygoneModeFill = false;
    }
}

void OGL_funcs::resizeGL(int w, int h)
{
    aCameraSetter.screenWidth = w;
    aCameraSetter.screenHeight = h;
    glViewport(0, 0, w, h);
}


#include <QKeyEvent>
void OGL_funcs::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Right)
    {
        aTextureHolder.moreOpacityForFirstTexture();
    }
    else if (event->key() == Qt::Key_Left)
    {
        aTextureHolder.moreOpacityForSecondTexture();
    }
    else if (event->key() == Qt::Key_1)
    {
        isChangePolygoneMode = true;
        isPolygoneModeLine = true;
    }
    else if (event->key() == Qt::Key_2)
    {
        isChangePolygoneMode = true;
        isPolygoneModeFill = true;
    }

    updateGL();
}

