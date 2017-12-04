#include "texture_holder.h"
#include <QGLFunctions>

typedef TextureHolder cls;

namespace {

quint32 texId[2] = {0};

}


cls::TextureHolder(QObject *parent) : AccessToQtOpenGl(parent)
{

}

void cls::initialize(const QGLContext *curContext,
                     QGLFunctions *funcs,
                     ShaderProgramSet *prog)
{
    AccessToQtOpenGl::initialize(curContext, funcs, prog);

    textureSettings();
    generateTextures();
}

void cls::doPaintWork()
{
    programSet->useProgram();
    programSet->setUniformValue("mixValue", mixValueFromUser);


    //    glEnable(GL_BLEND);
    //    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    f->glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texId[0]);
    f->glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texId[1]);
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

    programSet->useProgram();

    qint32 activeTextureIndex = 0;
    programSet->setUniformValue("ourTexture1", activeTextureIndex);
    activeTextureIndex = 1;
    programSet->setUniformValue("ourTexture2", activeTextureIndex);
    programSet->setUniformValue("mixValue", mixValueFromUser);

}

void cls::generateTextures()
{
    quint32 texCountForGen = 2;
    glGenTextures(texCountForGen, texId);

    QImage img(":/images/firstTexture.png");
    QImage img2(":/images/secondTex.png");
    img = img.mirrored();
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

    f->glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texId[0]);

    glTexImage2D(GL_TEXTURE_2D, mipmapLevel,
                 GL_RGBA, img.width(), img.height(),
                 borderLegacyStuff, GL_BGRA, GL_UNSIGNED_BYTE,
                 img.bits());
    f->glGenerateMipmap(GL_TEXTURE_2D);

    f->glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texId[1]);

    glTexImage2D(GL_TEXTURE_2D, mipmapLevel,
                 GL_RGBA, img2.width(), img2.height(),
                 borderLegacyStuff, GL_BGRA, GL_UNSIGNED_BYTE,
                 img2.bits());
    f->glGenerateMipmap(GL_TEXTURE_2D);

}


void cls::moreOpacityForFirstTexture()
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

void cls::moreOpacityForSecondTexture()
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

