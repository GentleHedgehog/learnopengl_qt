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

signals:

public slots:

private:
    const QGLContext *context = 0;
    QGLFunctions *f;
    ShaderProgramSet *programSet;
};

#endif // CAMERA_SETTER_H
