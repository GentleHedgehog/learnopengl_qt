#include "texture_holder.h"
#include <QGLFunctions>
#include "UsableClass/Macros/macros.h"


typedef TextureHolder cls;

namespace {

}


cls::TextureHolder(QObject *parent) :
    AccessToQtOpenGl(parent),
    textureDiffuseMap(QOpenGLTexture::Target2D),
    textureSpecularMap(QOpenGLTexture::Target2D)
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
//    DEBUG_NM("do paint");
    textureDiffuseMap.bind(0);
    textureSpecularMap.bind(1);
}

void cls::textureSettings()
{
//    DEBUG_NM("settings");

    textureDiffuseMap.setWrapMode(QOpenGLTexture::MirroredRepeat);
    // linearly interpolates between the
    // two closest mipmaps and samples
    // the texture via linear interpolation:
    textureDiffuseMap.setMagnificationFilter(QOpenGLTexture::LinearMipMapLinear);
    textureDiffuseMap.setMagnificationFilter(QOpenGLTexture::Linear);

    textureSpecularMap.setWrapMode(QOpenGLTexture::MirroredRepeat);
    textureSpecularMap.setMagnificationFilter(QOpenGLTexture::LinearMipMapLinear);
    textureSpecularMap.setMagnificationFilter(QOpenGLTexture::Linear);

    programSet->use();
    programSet->setUniformValue("material.diffuse", indexForDiffuseMap);
    programSet->setUniformValue("material.specular", indexForSpecularMap);

}

void cls::generateTextures()
{
//    DEBUG_NM("gen");
    assert (textureDiffuseMap.create());
    assert (textureSpecularMap.create());

    QImage img(":/images/metal_wood_box.png");
    img = img.mirrored();

    QImage img2(":/images/metal_wood_box_specular_map.png");
    img2 = img2.mirrored();

    textureDiffuseMap.bind(indexForDiffuseMap);
    textureDiffuseMap.setData(img);

    textureSpecularMap.bind(indexForSpecularMap);
    textureSpecularMap.setData(img2);

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

