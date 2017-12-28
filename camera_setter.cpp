#include "camera_setter.h"
#include "shader_data.h"
#include <QtMath>
#include <QTime>
#include "UsableClass/Macros/macros.h"

typedef CameraSetter cls;

cls::CameraSetter(QObject *parent) : AccessToQtOpenGl(parent)
{

}

void cls::updateMatrices()
{
    updateCurrentViewMatrix();
    updateCurrentProjMatrix();
    aMatrixHelper.loadMatrixToShader(programSet);
}

void cls::initialize(ShaderProgramSet *prog)
{
    AccessToQtOpenGl::initialize(prog);


    programSet->use();
//    QVector3D vecForModelRotation(1.0f, 0.0f, 0.0f);
//    aMatrixHelper.modelMat.rotate(-55.0f, vecForModelRotation);
//    aMatrixHelper.modelMat.translate(0.0f, 0.0f, 0.0f);
    aMatrixHelper.viewMat.translate(0.0f, 0.0f, 0.0f);
    aMatrixHelper.projMat.perspective(fov,
                                  (screenWidth / screenHeight),
                                  0.1f, 100.0f);

    aMatrixHelper.loadMatrixToShader(programSet);

    cameraPosition = QVector3D(0.0f, 0.0f, 3.0f);
    cameraFront = QVector3D(0.0f, 0.0f, -1.0f);
    cameraUp = QVector3D(0.0f, 1.0f, 0.0f);

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

QMatrix4x4 cls::updateCurrentProjMatrix()
{
    aMatrixHelper.projMat.setToIdentity();
    aMatrixHelper.projMat.perspective(fov,
                                  (screenWidth / screenHeight),
                                  0.1f, 100.0f);

    return aMatrixHelper.projMat;

}

QMatrix4x4 cls::updateCurrentViewMatrix()
{
    QVector3D eye = cameraPosition;
    QVector3D center/*(0.0f, 0.0f, 0.0f);*/ = cameraPosition + cameraFront;
    QVector3D up = cameraUp;

    aMatrixHelper.viewMat.setToIdentity();
    aMatrixHelper.viewMat.lookAt(eye, center, up);

    return aMatrixHelper.viewMat;
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


//void cls::calcNewDirection(float pitchDeg, float yawDeg)
//{
//    cameraDirection.setX(
//                qCos(qDegreesToRadians(pitch)) *
//                qCos(qDegreesToRadians(yaw)));

//    cameraDirection.setY(
//                qSin(qDegreesToRadians(pitch)));

//    cameraDirection.setZ(
//                qCos(qDegreesToRadians(pitch)) *
//                qSin(qDegreesToRadians(yaw)));
//}




