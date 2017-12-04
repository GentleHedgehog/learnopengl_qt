#ifndef CAMERA_SETTER_H
#define CAMERA_SETTER_H

#include <QVector3D>
#include <QMatrix4x4>
#include "access_to_qt_opengl.h"

class CameraSetter : public AccessToQtOpenGl
{
    Q_OBJECT
public:
    explicit CameraSetter(QObject *parent = 0);

    QVector3D cameraFront;
    float fov = 45.0f;

    QMatrix4x4 transformMatrix;
    QMatrix4x4 modelMatrix;
    QMatrix4x4 viewMatrix;
    QMatrix4x4 projectionMatrix;

    int screenWidth = 800;
    int screenHeight = 600;

    QMatrix4x4 getCurrentViewMatrix();
    QMatrix4x4 getCurrentProjMatrix();
    void moveCameraForward();
    void moveCameraBackward();
    void moveCameraRight();
    void moveCameraLeft();
    void notifyAboutNewFrame();

    void initialize(const QGLContext *curContext, QGLFunctions *funcs, ShaderProgramSet *prog) override;

signals:

public slots:

private:
    QVector3D cameraPosition;
    QVector3D cameraTarget;
    QVector3D cameraDirection;
    QVector3D cameraRight;
    QVector3D cameraUp;


    float cameraSpeed = 0.05f;
    float cameraSpeedBalanced = 0.05f;

    QMatrix4x4 lookAtMatrix;


    float lastFrameTimeMs = 0.0f;
    float deltaTimeMs = 0.0f;
};

#endif // CAMERA_SETTER_H
