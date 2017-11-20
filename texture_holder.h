#ifndef TEXTURE_HOLDER_H
#define TEXTURE_HOLDER_H

#include <QObject>
#include "shader_program_set.h"

class TextureHolder : public QObject
{
    Q_OBJECT
public:

    explicit TextureHolder(QObject *parent = 0);

    void initialize(const QGLContext *curContext,
                    QGLFunctions *funcs,
                    ShaderProgramSet *prog);

    void doPaintWork();

    void moreOpacityForFirstTexture();
    void moreOpacityForSecondTexture();

signals:

public slots:
private:
    float mixValueFromUser = 0.5;

    const QGLContext *context = 0;
    QGLFunctions *f;
    ShaderProgramSet *programSet;

    void textureSettings();
    void generateTextures();
};

#endif // TEXTURE_HOLDER_H
