#ifndef SHADER_PROGRAM_SET_H
#define SHADER_PROGRAM_SET_H


#include <QGLShader>
#include <QGLShaderProgram>

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

    void initialize(const QGLContext *curContext);

    void compile();

    void link();

    void useProgram();

    void deleteShaders();


private:
    bool isInitialize();
};

#endif // SHADER_PROGRAM_SET_H
