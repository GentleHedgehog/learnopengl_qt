#ifndef LIGHTING_H
#define LIGHTING_H

#include "access_to_qt_opengl.h"

class Lighting : public AccessToQtOpenGl
{
    Q_OBJECT
public:
    Lighting(QObject *parent=0);

    QOpenGLVertexArrayObject VAO;

    void initVAO(QOpenGLBuffer vbo, QOpenGLBuffer ebo);
    void doPaintWork();
};

#endif // LIGHTING_H
