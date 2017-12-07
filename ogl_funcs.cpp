#include "ogl_funcs.h"
#include "UsableClass/Macros/macros.h"
#include <QImage>

#include "vertex_data.h"
#include "shader_data.h"
#include "camera_setter.h"
#include "texture_holder.h"
#include "framebuffer.h"
#include "Lighting/lighting.h"

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
    Framebuffer aFramebuffer;

    float lastMouseX = 400, lastMouseY = 300;
    float pitch = 0, yaw = 0;

    bool isInitializeGl = false;

    Lighting aLighting;

    ShaderProgramSet *programUsual = 0;
}

cls::OGL_funcs(QWidget *parent) :
    QGLWidget(parent)
{
    setMouseTracking(true);
    grabKeyboard();


    resize(aCameraSetter.screenWidth, aCameraSetter.screenHeight);

    connect(&timer, SIGNAL(timeout()),
            this, SLOT(updateGL()) );
    timer.setInterval(0);
    timer.start();

    connect(&timerForFrameBuffer, SIGNAL(timeout()),
            this, SLOT(doRenderWorkAnywhere()) );
    timerForFrameBuffer.setInterval(2000);
//    timerForFrameBuffer.start();
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

void cls::setAttribs()
{
    aPos.applyAttrib(this);
    aColor.applyAttrib(this);
    aTextureCoord.applyAttrib(this);
}

void cls::initializeGL()
{
    if (isInitializeGl)
        return;
    isInitializeGl = true;

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
    setAttribs();
    VAO[0].release();

    DEBUG_NM(vertexShaderCode);
    DEBUG_NM(fragmentShaderCode);


    programUsual = new ShaderProgramSet(
                vertexShaderCode,
                fragmentShaderCode,
                QGLContext::currentContext(),
                this, this);

    programUsual->compile();
    assert(programUsual->link());


//    aTextureHolder.initialize(programUsual);

    aCameraSetter.initialize(programUsual);

//    aFramebuffer.initialize(programUsual);
//    aFramebuffer.create();

//    aLighting.initialize(programUsual);

//    aLighting.initVAO(VBO[0]);

}

void cls::paintGL()
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);

    programUsual->use();

//    DEBUG("repaint");

    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//    aTextureHolder.doPaintWork();
    aCameraSetter.notifyAboutNewFrame();

    VAO[0].bind();
//    glDrawArrays(GL_TRIANGLES, 0, 3);

//    programUsual->setUniformValue(objectColor.toUtf8().constData(), QVector3D(1.0f, 0.5f, 0.31f));
//    programUsual->setUniformValue(lightColor.toUtf8().constData(), QVector3D(1.0f, 1.0f, 1.0f));


    for (size_t i = 0; i < sizeofArray(cubesPositions); ++i)
    {
        QVector3D vecForModelTranslation = cubesPositions[i];
        QVector3D vecForModelRotation = QVector3D(0.0, 1.0, 0.0f);

        float rotateAngle = 0;//(float)(QTime::currentTime().msec() % 5);

//        cubesModelMatrices[i].setToIdentity();
        cubesModelMatrices[i].translate(vecForModelTranslation);
        cubesModelMatrices[i].rotate(rotateAngle, vecForModelRotation);

        aMatrixHelper.modelMat = cubesModelMatrices[i];
        aMatrixHelper.loadMatrixToShader(programUsual);

        cubesModelMatrices[i].translate(vecForModelTranslation * -1);

        glDrawElements(GL_TRIANGLES, 6*6, GL_UNSIGNED_INT, 0);
    }


    aCameraSetter.updateMatrices();


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

//    static bool isShowFramebuffer = true;

    if (false/*isShowFramebuffer*/)
    {
//        isShowFramebuffer = false;
        aFramebuffer.getImage();
    }
}

void cls::doRenderWorkAnywhere()
{
    makeCurrent();
    assert (this->isValid());

    static bool isInit = false;

    if (! isInit)
    {
        isInit = true;
        DEBUG("init anywhere");
        initializeGL();

        resizeGL(800, 600);
    }


    paintGL();

//    doneCurrent();
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
        aCameraSetter.moveCameraRight();
    }
    else if (event->key() == Qt::Key_Left)
    {
        aCameraSetter.moveCameraLeft();
    }
    else if (event->key() == Qt::Key_Up)
    {
        aCameraSetter.moveCameraForward();
    }
    else if (event->key() == Qt::Key_Down)
    {
        aCameraSetter.moveCameraBackward();
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

//    updateGL();
}

#include <QMouseEvent>
void OGL_funcs::mouseMoveEvent(QMouseEvent *event)
{
//    return;
    float xpos = event->pos().x();
    float ypos = event->pos().y();

    static bool firstMouse = true;
    if (firstMouse) // this bool variable is initially set to true
    {
//        lastMouseX = xpos;
//        lastMouseY = ypos;
        firstMouse = false;
    }


    float xoffset = xpos - lastMouseX;
    float yoffset = lastMouseY - ypos; // positive offset from bottom to top

    lastMouseX = xpos;
    lastMouseY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    pitch += yoffset;
    yaw += xoffset;

    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;
//    if(yaw > 89.0f)
//        yaw = 89.0f;
//    if(yaw < -89.0f)
//        yaw = -89.0f;


    float x = qCos(qDegreesToRadians(pitch)) * qSin(qDegreesToRadians(yaw));
    float y = qSin(qDegreesToRadians(pitch));
    float z = qCos(qDegreesToRadians(pitch)) * (-1 * qCos(qDegreesToRadians(yaw)));
    QVector3D front;
    front.setX(x);
    front.setY(y);
    front.setZ(z);
    front.normalize();

    // cameraFront: vector -> where camera looks at
    aCameraSetter.cameraFront = front.normalized();

//    DEBUG_NM(aCameraSetter.cameraFront);
//    DEBUG_NM(pitch);
//    DEBUG_NM(yaw);
}

void OGL_funcs::wheelEvent(QWheelEvent *event)
{
    float yoffset = event->angleDelta().y();


    if(aCameraSetter.fov >= 1.0f && aCameraSetter.fov <= 45.0f)
    {
        if (yoffset > 0)
        {
            aCameraSetter.fov += 2;
        }
        else
        {
            aCameraSetter.fov -= 2;
        }
    }
    if(aCameraSetter.fov <= 1.0f)
        aCameraSetter.fov = 1.0f;
    if(aCameraSetter.fov >= 45.0f)
        aCameraSetter.fov = 45.0f;

//    DEBUG_NM(yoffset);
//    DEBUG_NM(aCameraSetter.fov);
}

//void OGL_funcs::mouse(QKeyEvent *event)
//{

//}

