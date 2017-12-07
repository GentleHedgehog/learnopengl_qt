#include "shader_program_set.h"
#include "UsableClass/Macros/macros.h"

typedef ShaderProgramSet cls;

#define RETURN_IS_NOT_INITIALIZE(ret_val) do{\
    if (! isInitialize()){\
        DEBUG2("ShaderProgramSet - not initialized! ", __PRETTY_FUNCTION__);\
        return ret_val;\
    }\
    }while(0)

cls::ShaderProgramSet(QString vrtxShaderCode,
                      QString frgmntShaderCode,
                      const QGLContext *curContext,
                      QGLFunctions *funcs,
                      QObject *parent):
    QGLShaderProgram(curContext, parent)
{
    context = curContext;
    f = funcs;

    vertexShaderCode = vrtxShaderCode;
    fragmentShaderCode = frgmntShaderCode;

    vertexShader = new QGLShader(QGLShader::Vertex);
    fragmentShader = new QGLShader(QGLShader::Fragment);
}

void cls::initialize()
{
}

void cls::compile()
{
    RETURN_IS_NOT_INITIALIZE();

    bool ok = true;

    ok = vertexShader->compileSourceCode(vertexShaderCode);
    if (! ok)
    {
        DEBUG_NM(vertexShader->log());
    }

    ok = fragmentShader->compileSourceCode(fragmentShaderCode);
    if (! ok)
    {
        DEBUG_NM(fragmentShader->log());
    }
}

bool cls::link()
{
    RETURN_IS_NOT_INITIALIZE(false);

    addShader(vertexShader);
    addShader(fragmentShader);
    bool ok = QGLShaderProgram::link();
    if (! ok)
    {
        DEBUG_NM(log());
    }

    return ok;
}

void cls::use()
{
    RETURN_IS_NOT_INITIALIZE();
    bind();
}

void cls::deleteShaders()
{
}

bool cls::isInitialize()
{
    return ((context != 0) && (f != 0));
}
