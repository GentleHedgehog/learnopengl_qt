#ifndef OGL_FUNCS_H
#define OGL_FUNCS_H

#include <QGLWidget>
#include <QGLFunctions>

struct sShaderProgram{
    quint32 vertexShaderId = 0;
    quint32 fragmentShaderId = 0;
    quint32 shaderProgramId = 0;

    QString vertexShaderCode;
    QString fragmentShaderCode;

    sShaderProgram(QString vrtxShaderCode,
                   QString frgmntShaderCode)
    {
        vertexShaderCode = vrtxShaderCode;
        fragmentShaderCode = frgmntShaderCode;
    }
};


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
    void createShaders(sShaderProgram &prog);
    void createProgramWithShaders(sShaderProgram &prog);
    void deleteShaders(sShaderProgram &prog);
    void setAttribFroVertexAPos();


    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);
};

#endif // OGL_FUNCS_H
