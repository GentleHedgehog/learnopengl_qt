#include "ogl_funcs.h"
#include "UsableClass/Macros/macros.h"
#include <QOpenGLVertexArrayObject>


typedef OGL_funcs cls;

namespace
{
    float vertices_first_triangle[] = {
        // vertex           // color
        0.5f,  0.5f, 0.0f, 1.0, 0.0, 0.0,  // top
        0.1f,  0.0f, 0.0f, 0.0, 1.0, 0.0, // bottom left
        0.9f,  0.0f, 0.0f, 0.0, 0.0, 1.0, // bottom right
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
    const quint32 SHD_STRIDE_A_POS = 6 * sizeof(GL_FLOAT);
    const quint32 SHD_OFFSET_A_POS = 0;

    const quint32 SHD_LOCATION_A_COLOR = 1;
    const quint32 SHD_ELEMENT_COUNT_A_COLOR = 3;
    const quint32 SHD_STRIDE_A_COLOR = 6 * sizeof(GL_FLOAT);
    const quint32 SHD_OFFSET_A_COLOR = 3 * sizeof(GL_FLOAT);


    QString vertexShaderCode =
            "#version 330 core\n"
            "layout (location = "+
            QString::number(SHD_LOCATION_A_POS)+
            ") in vec3 aPos;\n"
            "layout (location = "+
            QString::number(SHD_LOCATION_A_COLOR)+
            ") in vec3 aColor;\n"
            "out vec4 usualColor;\n\n"
            "void main(){\n"
            "gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
            "usualColor = vec4(aPos, 1.0);\n"
            "}";

    ShaderProgramSet programUsual(
            vertexShaderCode
            ,

            "#version 330 core\n"
            "in vec4 usualColor;\n"
            "out vec4 color;\n\n"
            "void main(){\n"
            "color = usualColor;\n"
            "}"
            );

    ShaderProgramSet programYellow(
            vertexShaderCode
            ,

            "#version 330 core\n"
            "uniform vec4 uniformColor;\n"
            "out vec4 color;\n\n"
            "void main(){\n"
            "color = uniformColor;\n"
            "}"
            );


    QOpenGLVertexArrayObject VAO[2];
    quint32 EBO[2] = {0};
    quint32 VBO[2] = {0};
}

cls::OGL_funcs(QWidget *parent) :
    QGLWidget(parent)
{
    resize(200, 200);
}

void cls::createBufObjectsForVertices(bool isFirstTriangle)
{
    if (isFirstTriangle)
    {
        quint32 bufCountForGen = 2;
        glGenBuffers(bufCountForGen, VBO);
        glGenBuffers(bufCountForGen, EBO);

//        DEBUG_NM(VBO[0]);
//        DEBUG_NM(EBO[0]);
//        DEBUG_NM(VBO[1]);
//        DEBUG_NM(EBO[1]);

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

void cls::setAttribFroVertexAPos()
{
    glVertexAttribPointer(SHD_LOCATION_A_POS,
                          SHD_ELEMENT_COUNT_A_POS,
                          GL_FLOAT, GL_FALSE,
                          SHD_STRIDE_A_POS,
                          (void*)SHD_OFFSET_A_POS);
    glEnableVertexAttribArray(SHD_LOCATION_A_POS);

    glVertexAttribPointer(SHD_LOCATION_A_COLOR,
                          SHD_ELEMENT_COUNT_A_COLOR,
                          GL_FLOAT, GL_FALSE,
                          SHD_STRIDE_A_COLOR,
                          (void*)SHD_OFFSET_A_COLOR);
    glEnableVertexAttribArray(SHD_LOCATION_A_COLOR);
}

void cls::initializeGL()
{
//    DEBUG(__PRETTY_FUNCTION__);


    initializeGLFunctions(QGLContext::currentContext());

    glClearColor(1.0, 1.0, 1.0, 1.0);

//    qint32 nMaxVertexAttribs = 0;
//    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nMaxVertexAttribs);
//    DEBUG_NM(nMaxVertexAttribs);

//    DEBUG_NM(QGLContext::currentContext()->
//             format().majorVersion());
//    DEBUG_NM(QGLContext::currentContext()->
//             format().minorVersion());

    assert(VAO[0].create());
    VAO[0].bind();
    createBufObjectsForVertices(true);
    setAttribFroVertexAPos();
    VAO[0].release();


//    assert(VAO[1].create());
//    VAO[1].bind();
//    createBufObjectsForVertices(false);
//    setAttribFroVertexAPos();
//    VAO[1].release();

    programUsual.initialize(
                QGLContext::currentContext());
    programUsual.compile();
    programUsual.link();

//    createShaders(programYellow);
//    createProgramWithShaders(programYellow);
//    deleteShaders(programYellow);

}

void cls::paintGL()
{
    programUsual.useProgram();
    VAO[0].bind();
//    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
    VAO[0].release();

//    glUseProgram(programYellow.shaderProgramId);

//    qint32 currentTimeSec = QTime::currentTime().second();
//    float greenPart = qSin(currentTimeSec) / 2.0f + 0.5f;
//    GLint uniformVarId = glGetUniformLocation(
//                programYellow.shaderProgramId, "uniformColor");
//    glUniform4f(uniformVarId, 0.0f, greenPart, 0.0f, 1.0f);

//    VAO[1].bind();
////    glDrawArrays(GL_TRIANGLES, 0, 3);
//    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
//    VAO[1].release();
}

void OGL_funcs::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    if (h > 400)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

