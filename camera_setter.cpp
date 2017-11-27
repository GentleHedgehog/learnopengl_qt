#include "camera_setter.h"
#include "shader_data.h"
#include <QtMath>
#include <QTime>
#include "UsableClass/Macros/macros.h"

typedef CameraSetter cls;

cls::CameraSetter(QObject *parent) : QObject(parent)
{

}

void cls::initialize(const QGLContext *curContext,
                     QGLFunctions *funcs,
                     ShaderProgramSet *prog)
{
    context = curContext;
    f = funcs;
    programSet = prog;



    programSet->useProgram();
//    QVector3D vecForModelRotation(1.0f, 0.0f, 0.0f);
//    modelMatrix.rotate(-55.0f, vecForModelRotation);
    viewMatrix.translate(0.0f, 0.0f, 0.0f);
    projectionMatrix.perspective(45.0f,
                                  (screenWidth / screenHeight),
                                  0.1f, 100.0f);
//    projectionMatrix.ortho(0.0f, screenWidth,
//                           0.0f, screenHeight,
//                           0.1f, 100.0f);

    programSet->setUniformMatrixValue(SHD_MODEL_MATRIX_NAME,
                                       modelMatrix.constData());
    programSet->setUniformMatrixValue(SHD_VIEW_MATRIX_NAME,
                                       viewMatrix.constData());
    programSet->setUniformMatrixValue(SHD_PROJ_MATRIX_NAME,
                                       projectionMatrix.constData());


    cameraPosition = QVector3D(0.0f, 0.0f, 3.0f);
    cameraFront = QVector3D(0.0f, 0.0f, -1.0f);
    cameraUp = QVector3D(0.0f, 1.0f, 0.0f);;

//    cameraPosition = QVector3D(0.0f, 0.0f, -3.0f);
//    cameraTarget = QVector3D(0.0f, 0.0f, 0.0f);
//    cameraDirection = (cameraPosition - cameraTarget).normalized();

//    QVector3D worldUpVector(0.0f, 1.0f, 0.0f);
//    cameraRight = QVector3D::crossProduct(worldUpVector,
//                                       cameraDirection).normalized();

//    cameraUp = QVector3D::crossProduct(cameraDirection,
//                                       cameraRight);

//    lookAtMatrix.lookAt(cameraPosition, cameraTarget, cameraUp);
}

QMatrix4x4 cls::getCurrentViewMatrix()
{
//    static int angle = 0;
    QMatrix4x4 view;
//    int radius = 10.0f;
//    angle++;
//    float x = qSin(qDegreesToRadians((float)(angle % 360))) * radius;
//    float z = qCos(qDegreesToRadians((float)(angle % 360))) * radius;

    QVector3D eye = cameraPosition;
    QVector3D center/*(0.0f, 0.0f, 0.0f);*/ = cameraPosition + cameraFront;
    QVector3D up = cameraUp;
    view.lookAt(eye, center, up);

    return view;
}

void cls::moveCameraForward()
{
    cameraPosition += cameraFront * cameraSpeedBalanced;
}

void cls::moveCameraBackward()
{
    cameraPosition -= cameraFront * cameraSpeedBalanced;
}

void cls::moveCameraRight()
{
    cameraPosition +=
            QVector3D::crossProduct(cameraFront,
                                    cameraUp).
            normalized() *
            cameraSpeedBalanced;
}

void cls::moveCameraLeft()
{
    cameraPosition -=
            QVector3D::crossProduct(cameraFront,
                                    cameraUp).
            normalized() *
            cameraSpeedBalanced;
}

void cls::notifyAboutNewFrame()
{
    float curTime = QTime::currentTime().msecsSinceStartOfDay();
    deltaTimeMs = curTime - lastFrameTimeMs;
//    DEBUG_NM(curTime);
//    DEBUG_NM(deltaTimeMs);
    if (deltaTimeMs > 100.0f)
    {
        deltaTimeMs = 10.0f;
    }
    lastFrameTimeMs = curTime;

    if (deltaTimeMs > 0.0f)
    {
        cameraSpeedBalanced = cameraSpeed * deltaTimeMs;
    }
}


void cls::calcNewDirection(float pitchDeg, float yawDeg)
{
    cameraDirection.setX(
                qCos(qDegreesToRadians(pitch)) *
                qCos(qDegreesToRadians(yaw)));

    cameraDirection.setY(
                qSin(qDegreesToRadians(pitch)));

    cameraDirection.setZ(
                qCos(qDegreesToRadians(pitch)) *
                qSin(qDegreesToRadians(yaw)));
}




