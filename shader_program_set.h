#ifndef SHADER_PROGRAM_SET_H
#define SHADER_PROGRAM_SET_H


#include <QGLShader>
#include <QGLShaderProgram>
#include <QGLFunctions>

class ShaderProgramSet{

public:

    QGLShader *vertexShader = 0;
    QGLShader *fragmentShader = 0;
    QGLShaderProgram *shaderProgram = 0;
    const QGLContext *context = 0;

    QString vertexShaderCode;
    QString fragmentShaderCode;

    ShaderProgramSet(QString vrtxShaderCode,
                   QString frgmntShaderCode);

    void initialize(const QGLContext *curContext,
                    QGLFunctions *funcs);

    void compile();

    void link();

    void useProgram();

    void deleteShaders();

    void setUniformValue(QString name, qint32 value);
    void setUniformValue(QString name, float value);
    void setUniformMatrixValue(QString name, const float *columnMajorMatrixData);


private:
    bool isInitialize();

    QGLFunctions *f;
};

#endif // SHADER_PROGRAM_SET_H
