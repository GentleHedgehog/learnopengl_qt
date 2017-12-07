#ifndef SHADER_HELPERS_H
#define SHADER_HELPERS_H

#include <QObject>
#include "shader_program_set.h"

struct sVboHelper
{
//    enum eType{
//        VERTEX,
//        COLOR,
//        TEXTURE_COORDINATE,
//    };

//    eType type;
    QString name;
    GLuint location;
    GLuint elementCount;
    GLuint stride;
    GLuint offset;

    sVboHelper(/*eType type,*/
               QString name,
               GLuint location,
               GLuint elementCount,
               GLuint stride,
               GLuint offset):
//        type(type),
        name(name),
        location(location),
        elementCount(elementCount),
        stride(stride),
        offset(offset)
    {
    }

    QString location_str()
    {
        return QString::number(location);
    }


    void applyAttrib(QGLFunctions *f)
    {
        f->glVertexAttribPointer(location,
                                 elementCount,
                                 GL_FLOAT, GL_FALSE,
                                 stride,
                                 (void*)offset);
        f->glEnableVertexAttribArray(location);
    }

};

#undef VBO_HELPER
#define VBO_HELPER(name, location, elementCount, stride, offset) \
    sVboHelper name(""#name"", location, elementCount, stride, offset)

#undef VBO_HELPER_VERTEX
#define VBO_HELPER_VERTEX(name, location) \
    VBO_HELPER(name, location, 3, 3 * sizeof(GL_FLOAT), 0)

#undef VBO_HELPER_VERTEX_COLOR
#define VBO_HELPER_VERTEX_COLOR(nameVertex, locationVertex, \
                                nameColor, locationColor) \
    VBO_HELPER(nameVertex, locationVertex, 3, 6 * sizeof(GL_FLOAT), 0);\
    VBO_HELPER(nameColor, locationColor, 3, 6 * sizeof(GL_FLOAT), 3 * sizeof(GL_FLOAT))

#undef VBO_HELPER_VERTEX_COLOR_TEXTURE
#define VBO_HELPER_VERTEX_COLOR_TEXTURE(nameVertex, locationVertex, \
                                        nameColor, locationColor,\
                                        nameTexture, locationTexture) \
    VBO_HELPER(nameVertex, locationVertex, 3, 8 * sizeof(GL_FLOAT), 0);\
    VBO_HELPER(nameColor, locationColor, 3, 8 * sizeof(GL_FLOAT), 3 * sizeof(GL_FLOAT));\
    VBO_HELPER(nameTexture, locationTexture, 2, 8 * sizeof(GL_FLOAT), 6 * sizeof(GL_FLOAT))





template <class MatrixType>
struct sMatrixHelper{

    static quint32 counter()
    {
        static quint32 counter = 0;
        return counter++;
    }

    QString model;
    QString view ;
    QString projection ;

    MatrixType modelMat;
    MatrixType viewMat;
    MatrixType projMat;

    sMatrixHelper()
    {
        model = "modelMatrix" + QString::number(counter());
        view = "viewMatrix" + QString::number(counter());
        projection = "projectionMatrix" + QString::number(counter());
    }

    const char* model_cstr()
    {
        return model.toUtf8().constData();
    }

    const char* view_cstr()
    {
        return view.toUtf8().constData();
    }

    const char* projection_cstr()
    {
        return projection.toUtf8().constData();
    }

    void loadMatrixToShader(ShaderProgramSet *prog)
    {
        prog->use();

        prog->setUniformValue(model_cstr(),
                              modelMat);
        prog->setUniformValue(view_cstr(),
                              viewMat);
        prog->setUniformValue(projection_cstr(),
                              projMat);
    }
};

struct sShaderHelper{
    QString text;

    sShaderHelper(QString text) : text(text)
    {
    }
};

#endif // SHADER_HELPERS_H
