#ifndef SHADER_HELPERS_H
#define SHADER_HELPERS_H

#include <QObject>
#include "shader_program_set.h"

struct sVertexAttributesHelper
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

    sVertexAttributesHelper(/*eType type,*/
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

#undef VERTEX_ATTRIBUTES_HELPER
#define VERTEX_ATTRIBUTES_HELPER(name, location, elementCount,\
                                 stride, offset) \
    sVertexAttributesHelper name(""#name"", location, elementCount, \
                                 stride, offset)

#undef VA_HELPER_VERTEX
#define VA_HELPER_VERTEX(name, location) \
    VERTEX_ATTRIBUTES_HELPER(name, location, \
                             3, 3 * sizeof(GL_FLOAT), 0)

#undef VA_HELPER_VERTEX_COLOR
#define VA_HELPER_VERTEX_COLOR(nameVertex, locationVertex, \
                                nameColor, locationColor) \
    VERTEX_ATTRIBUTES_HELPER(nameVertex, locationVertex, \
                             3, 6 * sizeof(GL_FLOAT), 0);\
    VERTEX_ATTRIBUTES_HELPER(nameColor, locationColor, \
                             3, 6 * sizeof(GL_FLOAT), \
                             3 * sizeof(GL_FLOAT))

#undef VA_HELPER_VERTEX_COLOR_TEXTURE
#define VA_HELPER_VERTEX_COLOR_TEXTURE(nameVertex, locationVertex, \
                                        nameColor, locationColor,\
                                        nameTexture, locationTexture) \
    VERTEX_ATTRIBUTES_HELPER(nameVertex, locationVertex, \
                             3, 8 * sizeof(GL_FLOAT), 0);\
    VERTEX_ATTRIBUTES_HELPER(nameColor, locationColor, \
                             3, 8 * sizeof(GL_FLOAT), \
                             3 * sizeof(GL_FLOAT));\
    VERTEX_ATTRIBUTES_HELPER(nameTexture, locationTexture,\
                             2, 8 * sizeof(GL_FLOAT), \
                             6 * sizeof(GL_FLOAT))



#undef VA_HELPER_VERTEX_NORMAL
#define VA_HELPER_VERTEX_NORMAL(nameVertex, locationVertex, \
                                nameNormal, locationNormal) \
    VA_HELPER_VERTEX_COLOR(nameVertex, locationVertex, \
                           nameNormal, locationNormal)

#undef VA_HELPER_VERTEX_NORMAL_TEXTURE
#define VA_HELPER_VERTEX_NORMAL_TEXTURE(nameVertex, locationVertex, \
                                        nameNormal, locationNormal,\
                                        nameTexture, locationTexture) \
    VA_HELPER_VERTEX_COLOR_TEXTURE(nameVertex, locationVertex, \
                                   nameNormal, locationNormal,\
                                   nameTexture, locationTexture)





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
