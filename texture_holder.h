#ifndef TEXTURE_HOLDER_H
#define TEXTURE_HOLDER_H

#include <QObject>
#include "access_to_qt_opengl.h"
#include <QOpenGLTexture>

class TextureHolder : public AccessToQtOpenGl
{
    Q_OBJECT
public:

    explicit TextureHolder(QObject *parent = 0);

    void initialize(ShaderProgramSet *prog) override;

    void doPaintWork();

    void moreOpacityForFirstTexture();
    void moreOpacityForSecondTexture();

signals:

public slots:
private:
    QOpenGLTexture textureDiffuseMap;
    QOpenGLTexture textureSpecularMap;
    qint32 indexForDiffuseMap = 0;
    qint32 indexForSpecularMap = 1;


    float mixValueFromUser = 0.5;

    void textureSettings();
    void generateTextures();
};

#endif // TEXTURE_HOLDER_H
