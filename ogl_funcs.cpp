#include "ogl_funcs.h"
#include "UsableClass/Macros/macros.h"
#include <QOpenGLVertexArrayObject>

typedef OGL_funcs cls;

namespace
{
    float vertices_first_triangle[] = {
        -0.5f,  0.5f, 0.0f, // top
        -0.9f, -0.5f, 0.0f, // bottom left
        0.0f, -0.5f, 0.0f, // bottom right
    };

    float vertices_second_triangle[] = {
        0.5f,  0.5f, 0.0f, // top
        0.0f, -0.5f, 0.0f, // bottom left
        0.9f, -0.5f, 0.0f, // bottom right
    };

    unsigned int indices_first_triangle[] = {
        0, 1, 2,
    };

    unsigned int indices_second_triangle[] = {
        0, 1, 2,
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

    QOpenGLVertexArrayObject VAO[2];
    quint32 EBO[2] = {0};
    quint32 VBO[2] = {0};
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

void cls::createBufObjectsForVertices(bool isFirstTriangle)
{
    if (isFirstTriangle)
    {
        quint32 bufCountForGen = 2;
        glGenBuffers(bufCountForGen, VBO);
        glGenBuffers(bufCountForGen, EBO);

        DEBUG_NM(VBO[0]);
        DEBUG_NM(EBO[0]);
        DEBUG_NM(VBO[1]);
        DEBUG_NM(EBO[1]);

        glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_first_triangle),
                     static_cast<void*>(vertices_first_triangle), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_first_triangle),
                     static_cast<void*>(indices_first_triangle), GL_STATIC_DRAW);
    }
    else
    {
        glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_second_triangle),
                     static_cast<void*>(vertices_second_triangle), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_second_triangle),
                     static_cast<void*>(indices_second_triangle), GL_STATIC_DRAW);
    }

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

    assert(VAO[0].create());
    VAO[0].bind();
    createBufObjectsForVertices(true);
    setAttribFroVertexAPos();
    VAO[0].release();


    assert(VAO[1].create());
    VAO[1].bind();
    createBufObjectsForVertices(false);
    setAttribFroVertexAPos();
    VAO[1].release();


    createShaders();

    createProgramWithShaders();

    deleteShaders();


}

void cls::paintGL()
{
    glUseProgram(shaderProgramId);
    VAO[0].bind();
//    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
    VAO[0].release();

    VAO[1].bind();
//    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
    VAO[1].release();
}

void OGL_funcs::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    if (h > 200)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}
