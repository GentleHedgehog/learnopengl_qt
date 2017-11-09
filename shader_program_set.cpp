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
                      QString frgmntShaderCode)
{
    vertexShaderCode = vrtxShaderCode;
    fragmentShaderCode = frgmntShaderCode;
}

void cls::initialize(const QGLContext *curContext)
{
    context = curContext;
    shaderProgram = new QGLShaderProgram(context);
    vertexShader = new QGLShader(QGLShader::Vertex);
    fragmentShader = new QGLShader(QGLShader::Fragment);
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

void cls::link()
{
    RETURN_IS_NOT_INITIALIZE();

    shaderProgram->addShader(vertexShader);
    shaderProgram->addShader(fragmentShader);
    bool ok = shaderProgram->link();
    if (! ok)
    {
        DEBUG_NM(shaderProgram->log());
    }
}

void cls::useProgram()
{
    RETURN_IS_NOT_INITIALIZE();
    shaderProgram->bind();
}

void cls::deleteShaders()
{
}

bool cls::isInitialize()
{
    return (shaderProgram != 0);
}
