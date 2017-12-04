#ifndef ACCESSTOQTOPENGL_H
#define ACCESSTOQTOPENGL_H

#include "shader_program_set.h"

class AccessToQtOpenGl : public QObject
{
    Q_OBJECT
public:
    AccessToQtOpenGl(QObject *parent=0);

    virtual void initialize(const QGLContext *curContext,
                         QGLFunctions *funcs,
                         ShaderProgramSet *prog)
    {
        context = curContext;
        f = funcs;
        programSet = prog;
    }

protected:
    const QGLContext *context = 0;
    QGLFunctions *f;
    ShaderProgramSet *programSet;
};

#endif // ACCESSTOQTOPENGL_H
