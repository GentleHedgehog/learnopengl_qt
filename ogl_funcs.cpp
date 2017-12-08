#include "ogl_funcs.h"
#include "UsableClass/Macros/macros.h"
#include <QImage>

#include "vertex_data.h"
#include "shader_data.h"
#include "camera_setter.h"
#include "framebuffer.h"
#include "Lighting/lighting.h"
#include <QOpenGLBuffer>

typedef OGL_funcs cls;

namespace
{
    QOpenGLVertexArrayObject VAO[2];
    QOpenGLBuffer vbo(QOpenGLBuffer::VertexBuffer);
    QOpenGLBuffer ebo(QOpenGLBuffer::IndexBuffer);

    bool isPolygoneModeLine = false;
    bool isPolygoneModeFill = true;
    bool isChangePolygoneMode = false;

    CameraSetter aCameraSetter;

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

void cls::createBufObjectsForVertices()
{
    assert (vbo.create());
    assert (vbo.bind());
    vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    vbo.allocate(static_cast<void*>(vertices_first_cube),
                 sizeof(vertices_first_cube));

    assert (ebo.create());
    assert (ebo.bind());
    ebo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    ebo.allocate(static_cast<void*>(indices_first_cube),
                 sizeof(indices_first_cube));

}

void cls::setAttribs()
{
    aPos.applyAttrib(this);
    aNormal.applyAttrib(this);
    aTexture.applyAttrib(this);
}

void cls::initializeGL()
{
    if (isInitializeGl)
        return;
    isInitializeGl = true;

//    DEBUG(__PRETTY_FUNCTION__);

    initializeGLFunctions(QGLContext::currentContext());

    qglClearColor(Qt::black);

//    qint32 nMaxVertexAttribs = 0;
//    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nMaxVertexAttribs);
//    DEBUG_NM(nMaxVertexAttribs);

//    DEBUG_NM(QGLContext::currentContext()->
//             format().majorVersion());
//    DEBUG_NM(QGLContext::currentContext()->
//             format().minorVersion());


    assert(VAO[0].create());
    VAO[0].bind();
    createBufObjectsForVertices();
    setAttribs();
    VAO[0].release();

    programUsual = new ShaderProgramSet(
                vertexShaderCode,
                fragmentShaderCode,
                QGLContext::currentContext(),
                this, this);

    programUsual->compile();
    assert(programUsual->link());


    aTextureHolder.initialize(programUsual);

    aCameraSetter.initialize(programUsual);

//    aFramebuffer.initialize(programUsual);
//    aFramebuffer.create();

    aLighting.initialize(programUsual);

    aLighting.initVAO(vbo, ebo);

}

void OGL_funcs::render()
{
    programUsual->use();
    programUsual->setUniformValue("lightPos",
                                  aLighting.lightPos);
    programUsual->setUniformValue("light.directionFromLightSource",
                                  QVector3D(2.0f, -1.0f, -0.3f));
    programUsual->setUniformValue("cameraPos",
                                  aCameraSetter.cameraPosition);


    VAO[0].bind();
//    glDrawArrays(GL_TRIANGLES, 0, 3);

    for (size_t i = 0; i < sizeofArray(cubesPositions); ++i)
    {
        QVector3D vecForModelTranslation = cubesPositions[i];
        QVector3D vecForModelRotation = QVector3D(0.0, 1.0, 0.0f);

        float rotateAngle = 0.009;//(float)(QTime::currentTime().msec() % 5);

//        cubesModelMatrices[i].setToIdentity();
        cubesModelMatrices[i].translate(vecForModelTranslation);
        cubesModelMatrices[i].rotate(rotateAngle, vecForModelRotation);

        aMatrixHelper.modelMat = cubesModelMatrices[i];
        aMatrixHelper.loadMatrixToShader(programUsual);

        cubesModelMatrices[i].translate(vecForModelTranslation * -1);

        glDrawElements(GL_TRIANGLES, 6*6, GL_UNSIGNED_INT, 0);
    }
    VAO[0].release();
}

void cls::paintGL()
{
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    render();

    aTextureHolder.doPaintWork();
    aCameraSetter.notifyAboutNewFrame();
    aCameraSetter.updateMatrices();

    aLighting.doPaintWork();

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

