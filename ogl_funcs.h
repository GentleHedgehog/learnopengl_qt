#ifndef OGL_FUNCS_H
#define OGL_FUNCS_H

#include <QGLWidget>
#include <QGLFunctions>
#include "shader_program_set.h"



class OGL_funcs :
        public QGLWidget,
        public QGLFunctions
{
    Q_OBJECT
public:
    OGL_funcs(QWidget *parent = 0);


private:

    void createBufObjectsForVertices(bool isFirstTriangle);
    void setAttribFroVertexAPos();


    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);
};

#endif // OGL_FUNCS_H
