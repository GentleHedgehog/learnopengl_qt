#include "texture_holder.h"
#include <QGLFunctions>
#include "UsableClass/Macros/macros.h"
#include <QOpenGLTexture>

typedef TextureHolder cls;

namespace {

QOpenGLTexture texture(QOpenGLTexture::Target2D);
qint32 activeTextureIndex = 0;
}


cls::TextureHolder(QObject *parent) : AccessToQtOpenGl(parent)
{

}

void cls::initialize(ShaderProgramSet *prog)
{
    AccessToQtOpenGl::initialize(prog);

    textureSettings();
    generateTextures();
}

void cls::doPaintWork()
{
    texture.bind(0);
}

void cls::textureSettings()
{
    texture.setWrapMode(QOpenGLTexture::MirroredRepeat);
    // linearly interpolates between the
    // two closest mipmaps and samples
    // the texture via linear interpolation:
    texture.setMagnificationFilter(QOpenGLTexture::LinearMipMapLinear);
    texture.setMagnificationFilter(QOpenGLTexture::Linear);

    programSet->use();
    programSet->setUniformValue("material.diffuse", activeTextureIndex);

}

void cls::generateTextures()
{
    assert (texture.create());

    QImage img(":/images/metal_wood_box.png");
    img = img.mirrored();

    texture.bind(activeTextureIndex);

    texture.setData(img);

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

