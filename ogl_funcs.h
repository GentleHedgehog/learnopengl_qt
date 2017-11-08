#ifndef OGL_FUNCS_H
#define OGL_FUNCS_H

#include <QGLWidget>
#include <QGLFunctions>

class OGL_funcs :
        public QGLWidget,
        public QGLFunctions
{
    Q_OBJECT
public:
    OGL_funcs(QWidget *parent = 0);


private:
    quint32 createShader(GLenum type, QString source);
    void createBufObjectsForVertices(bool isFirstTriangle);
    void createShaders();
    void createProgramWithShaders();
    void deleteShaders();
    void setAttribFroVertexAPos();


    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);
};

#endif // OGL_FUNCS_H
