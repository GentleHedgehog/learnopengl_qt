#ifndef TEXTURE_HOLDER_H
#define TEXTURE_HOLDER_H

#include <QObject>
#include "access_to_qt_opengl.h"

class TextureHolder : public AccessToQtOpenGl
{
    Q_OBJECT
public:

    explicit TextureHolder(QObject *parent = 0);

    void initialize(const QGLContext *curContext,
                    QGLFunctions *funcs,
                    ShaderProgramSet *prog) override;

    void doPaintWork();

    void moreOpacityForFirstTexture();
    void moreOpacityForSecondTexture();

signals:

public slots:
private:
    float mixValueFromUser = 0.5;

    void textureSettings();
    void generateTextures();
};

#endif // TEXTURE_HOLDER_H
