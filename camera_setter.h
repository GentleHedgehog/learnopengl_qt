#ifndef CAMERA_SETTER_H
#define CAMERA_SETTER_H

#include <QObject>
#include <QVector3D>
#include <QMatrix4x4>
#include "shader_program_set.h"

class CameraSetter : public QObject
{
    Q_OBJECT
public:
    explicit CameraSetter(QObject *parent = 0);


    QMatrix4x4 transformMatrix;
    QMatrix4x4 modelMatrix;
    QMatrix4x4 viewMatrix;
    QMatrix4x4 projectionMatrix;

    int screenWidth = 400;
    int screenHeight = 400;

    void initialize(const QGLContext *curContext,
                    QGLFunctions *funcs,
                    ShaderProgramSet *prog);

    QMatrix4x4 getCurrentViewMatrix();
    void moveCameraForward();
    void moveCameraBackward();
    void moveCameraRight();
    void moveCameraLeft();
signals:

public slots:

private:
    const QGLContext *context = 0;
    QGLFunctions *f;
    ShaderProgramSet *programSet;

    QVector3D cameraPosition;
    QVector3D cameraTarget;
    QVector3D cameraDirection;
    QVector3D cameraRight;
    QVector3D cameraUp;

    QVector3D cameraFront;

    float cameraSpeed = 0.05;

    QMatrix4x4 lookAtMatrix;
};

#endif // CAMERA_SETTER_H
