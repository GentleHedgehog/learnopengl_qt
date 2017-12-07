#ifndef ACCESSTOQTOPENGL_H
#define ACCESSTOQTOPENGL_H

#include <QOpenGLVertexArrayObject>
#include "shader_program_set.h"

class AccessToQtOpenGl : public QObject
{
    Q_OBJECT
public:
    AccessToQtOpenGl(QObject *parent=0);

    virtual void initialize(ShaderProgramSet *prog)
    {
        programSet = prog;
        context = prog->context;
        f = prog->f;
    }

protected:
    ShaderProgramSet *programSet=0;
    const QGLContext *context=0;
    QGLFunctions *f=0;
};

#endif // ACCESSTOQTOPENGL_H
