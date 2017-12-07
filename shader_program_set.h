#ifndef SHADER_PROGRAM_SET_H
#define SHADER_PROGRAM_SET_H


#include <QGLShader>
#include <QGLShaderProgram>
#include <QGLFunctions>

class ShaderProgramSet : public QGLShaderProgram
{

public:

    QGLShader *vertexShader = 0;
    QGLShader *fragmentShader = 0;
    const QGLContext *context = 0;
    QGLFunctions *f = 0;

    QString vertexShaderCode;
    QString fragmentShaderCode;

    ShaderProgramSet(QString vrtxShaderCode,
                     QString frgmntShaderCode,
                     const QGLContext *curContext,
                     QGLFunctions *funcs,
                     QObject *parent = 0);

    void initialize();

    void compile();

    bool link() override;

    void use();

    void deleteShaders();


private:
    bool isInitialize();

};

#endif // SHADER_PROGRAM_SET_H
