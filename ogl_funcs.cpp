#include "ogl_funcs.h"
#include "UsableClass/Macros/macros.h"
#include <QOpenGLVertexArrayObject>
#include <QImage>


typedef OGL_funcs cls;

namespace
{
    float vertices_first_triangle[] = {
        // vertex               // color            // tex coord
       -1.0f,  1.0f, 0.0f,      1.0, 0.0, 0.0,      0.0f, 2.0f,// top left
        1.0f,  1.0f, 0.0f,      1.0, 0.0, 0.0,      2.0f, 2.0f,// top right
       -1.0f, -1.0f, 0.0f,      0.0, 1.0, 0.0,      0.0f, 0.0f,// bottom left
        1.0f, -1.0f, 0.0f,      0.0, 0.0, 1.0,      2.0f, 0.0f,// bottom right
    };

    float vertices_second_triangle[] = {
        0.5f,  0.5f, 0.0f, // top
        0.0f, -0.5f, 0.0f, // bottom left
        0.9f, -0.5f, 0.0f, // bottom right
    };

    unsigned int indices_first_triangle[] = {
        0, 1, 2,
        2, 3, 1,
    };

    unsigned int indices_second_triangle[] = {
        0, 1, 2,
    };

    const quint32 SHD_LOCATION_A_POS = 0;
    const quint32 SHD_ELEMENT_COUNT_A_POS = 3;
    const quint32 SHD_STRIDE_A_POS = 8 * sizeof(GL_FLOAT);
    const quint32 SHD_OFFSET_A_POS = 0;

    const quint32 SHD_LOCATION_A_COLOR = 1;
    const quint32 SHD_ELEMENT_COUNT_A_COLOR = 3;
    const quint32 SHD_STRIDE_A_COLOR = 8 * sizeof(GL_FLOAT);
    const quint32 SHD_OFFSET_A_COLOR = 3 * sizeof(GL_FLOAT);

    const quint32 SHD_LOCATION_A_TEX_COORD = 2;
    const quint32 SHD_ELEMENT_COUNT_A_TEX_COORD = 2;
    const quint32 SHD_STRIDE_A_TEX_COORD = 8 * sizeof(GL_FLOAT);
    const quint32 SHD_OFFSET_A_TEX_COORD = 6 * sizeof(GL_FLOAT);


    QString vertexShaderCode =
            "#version 330 core\n"
            "layout (location = "+
            QString::number(SHD_LOCATION_A_POS)+
            ") in vec3 aPos;\n"
            "layout (location = "+
            QString::number(SHD_LOCATION_A_COLOR)+
            ") in vec3 aColor;\n"
            "layout (location = "+
            QString::number(SHD_LOCATION_A_TEX_COORD)+
            ") in vec2 aTexCoord;\n"
            "out vec2 texCoord;\n"
            "out vec4 usualColor;\n\n"
            "void main(){\n"
            "gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
            "usualColor = vec4(aColor, 1.0);\n"
            "texCoord = aTexCoord;\n"
            "}";

    ShaderProgramSet programUsual(
            vertexShaderCode
            ,

            "#version 330 core\n"
            "in vec4 usualColor;\n"
            "in vec2 texCoord;\n"
            "out vec4 fragColor;\n\n"
            "uniform float mixValue;\n"
            "uniform sampler2D ourTexture1;\n"
            "uniform sampler2D ourTexture2;\n\n"
            "void main(){\n"
//            "fragColor = usualColor;\n"
            "vec4 color1 = texture(ourTexture1, texCoord);\n"
//            "vec4 color1 = vec4(1.0, 1.0, 1.0, 1.0);\n"
            "vec4 color2 = texture(ourTexture2, vec2(texCoord.x,texCoord.y));\n"
//            "vec4 color2 = vec4(1.0, 0.0, 0.0, 1.0);\n"
            "fragColor = mix("
            "color1.rgba,"
            "color2.rgba, "
            "mixValue);\n"
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
    quint32 texId[2] = {0};
    float mixValueFromUser = 0.5;
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

    glVertexAttribPointer(SHD_LOCATION_A_TEX_COORD,
                          SHD_ELEMENT_COUNT_A_TEX_COORD,
                          GL_FLOAT, GL_FALSE,
                          SHD_STRIDE_A_TEX_COORD,
                          (void*)SHD_OFFSET_A_TEX_COORD);
    glEnableVertexAttribArray(SHD_LOCATION_A_TEX_COORD);
}

void cls::textureSettings()
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    // linearly interpolates between the
    // two closest mipmaps and samples
    // the texture via linear interpolation:
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    programUsual.useProgram();

    glUniform1i(
                glGetUniformLocation(
                    programUsual.shaderProgram->programId(),
                    "ourTexture1"),
                0);

    glUniform1i(
                glGetUniformLocation(
                    programUsual.shaderProgram->programId(),
                    "ourTexture2"),
                1);

    glUniform1f(
                glGetUniformLocation(
                    programUsual.shaderProgram->programId(),
                    "mixValue"),
                mixValueFromUser);
}

void cls::generateTextures()
{
    quint32 texCountForGen = 2;
    glGenTextures(texCountForGen, texId);

    QImage img(":/images/firstTexture.png");
    QImage img2(":/images/secondTex.png");
    img2 = img2.mirrored();
//    DEBUG_NM(img.hasAlphaChannel());
//    DEBUG_NM(img2.hasAlphaChannel());
//    DEBUG_NM(img.format());  // 4
//    DEBUG_NM(img2.format()); // 5?

//    DEBUG_NM(img2.isNull());
//    DEBUG_NM(img2.size());

//    DEBUG_NM(img.format());
//    QImage::Format_RGB32
//    4
//    The image is stored using a 32-bit RGB format (0xffRRGGBB).


    const GLint mipmapLevel = 0;
    const GLint borderLegacyStuff = 0;

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texId[0]);

    glTexImage2D(GL_TEXTURE_2D, mipmapLevel,
                 GL_RGBA, img.width(), img.height(),
                 borderLegacyStuff, GL_BGRA, GL_UNSIGNED_BYTE,
                 img.bits());
    glGenerateMipmap(GL_TEXTURE_2D);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texId[1]);

    glTexImage2D(GL_TEXTURE_2D, mipmapLevel,
                 GL_RGBA, img2.width(), img2.height(),
                 borderLegacyStuff, GL_BGRA, GL_UNSIGNED_BYTE,
                 img2.bits());
    glGenerateMipmap(GL_TEXTURE_2D);

}

#include <QKeyEvent>
void OGL_funcs::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Right)
    {
        if ((mixValueFromUser + 0.005f) < 1.0f)
        {
            mixValueFromUser += 0.005;
        }
        else
        {
            mixValueFromUser = 1.0f;
        }
    }
    else if (event->key() == Qt::Key_Left)
    {
        if ((mixValueFromUser - 0.005f) > 0.0f)
        {
            mixValueFromUser -= 0.005;
        }
        else
        {
            mixValueFromUser = 0.0f;
        }
    }
    DEBUG_NM(mixValueFromUser);
    updateGL();
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

    textureSettings();
    generateTextures();



}

void cls::paintGL()
{
    programUsual.useProgram();
    DEBUG("repaint");

    glUniform1f(
                glGetUniformLocation(
                    programUsual.shaderProgram->programId(),
                    "mixValue"),
                mixValueFromUser);

    glClear(GL_COLOR_BUFFER_BIT);
//    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texId[0]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texId[1]);

    VAO[0].bind();
//    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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

