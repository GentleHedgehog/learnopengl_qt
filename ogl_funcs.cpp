#include "ogl_funcs.h"
#include "UsableClass/Macros/macros.h"
#include <QOpenGLVertexArrayObject>

typedef OGL_funcs cls;

namespace
{
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    const quint32 SHD_LOCATION_A_POS = 0;
    const quint32 SHD_ELEMENT_COUNT_A_POS = 3;
    const quint32 SHD_STRIDE_A_POS = 3 * sizeof(GL_FLOAT);
    const quint32 SHD_OFFSET_A_POS = 0;
    QString vertexShaderCode =
            "#version 330 core\n"
            "layout (location = "+
            QString::number(SHD_LOCATION_A_POS)+
            ") in vec3 aPos;\n\n"
            "void main(){\n"
            "gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
            "}";

    QString fragmentShaderCode =
            "#version 330 core\n"
            "out vec4 color;\n\n"
            "void main(){\n"
            "color = vec4(1.0, 0.5, 0.2, 1.0);\n"
            "}";

    QOpenGLVertexArrayObject VAO;
    quint32 VBO = 0;
    quint32 vertexShaderId = 0;
    quint32 fragmentShaderId = 0;
    quint32 shaderProgramId = 0;
}

cls::OGL_funcs(QWidget *parent) :
    QGLWidget(parent)
{
    resize(200, 200);
}

quint32 cls::createShader(GLenum type, QString source)
{
    quint32 shaderId = glCreateShader(type);
    quint32 stringCount = 1;
    const char *str = source.toUtf8().data();
    glShaderSource(shaderId, stringCount,
                   &str, NULL);
    glCompileShader(shaderId);

    int success = 0;
    char infoLog[512] = {0};
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shaderId, sizeof(infoLog),
                           NULL, infoLog);
        DEBUG_NM(infoLog);
    }

    return shaderId;
}

void cls::createVBOForVertices()
{
    quint32 bufCountForGen = 1;
    glGenBuffers(bufCountForGen, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),
                 static_cast<void*>(vertices), GL_STATIC_DRAW);
}

void cls::createShaders()
{
    vertexShaderId = createShader(GL_VERTEX_SHADER,
                                  vertexShaderCode);
    fragmentShaderId = createShader(GL_FRAGMENT_SHADER,
                                    fragmentShaderCode);
}

void cls::createProgramWithShaders()
{
    shaderProgramId = glCreateProgram();
    glAttachShader(shaderProgramId, vertexShaderId);
    glAttachShader(shaderProgramId, fragmentShaderId);
    glLinkProgram(shaderProgramId);


    int success = 0;
    char infoLog[512] = {0};
    glGetProgramiv(shaderProgramId, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgramId,
                            sizeof(infoLog),
                            NULL, infoLog);
        DEBUG_NM(infoLog);
    }
    else
    {
        glUseProgram(shaderProgramId);
    }
}

void cls::deleteShaders()
{
    glDeleteShader(vertexShaderId);
    vertexShaderId = 0;
    glDeleteShader(fragmentShaderId);
    fragmentShaderId = 0;
}

void cls::setAttribFroVertexAPos()
{
    glVertexAttribPointer(SHD_LOCATION_A_POS,
                          SHD_ELEMENT_COUNT_A_POS,
                          GL_FLOAT, GL_FALSE,
                          SHD_STRIDE_A_POS,
                          (void*)SHD_OFFSET_A_POS);
    glEnableVertexAttribArray(SHD_LOCATION_A_POS);
}

void cls::initializeGL()
{
//    DEBUG(__PRETTY_FUNCTION__);


    initializeGLFunctions(QGLContext::currentContext());

    //    DEBUG_NM(QGLContext::currentContext()->
    //             format().majorVersion());
    //    DEBUG_NM(QGLContext::currentContext()->
    //             format().minorVersion());

    assert(VAO.create());
    VAO.bind();

    createVBOForVertices();

    createShaders();

    createProgramWithShaders();

    deleteShaders();

    setAttribFroVertexAPos();

    VAO.release();
}

void cls::paintGL()
{
    glUseProgram(shaderProgramId);
    VAO.bind();
    glDrawArrays(GL_TRIANGLES, 0, 3);
    VAO.release();
}

void OGL_funcs::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}
