#include "framebuffer.h"
#include "UsableClass/Macros/macros.h"

typedef Framebuffer cls;

namespace  {


quint32 texture[2] = {0};

}

cls::Framebuffer()
{

}

bool cls::isComplete()
{
    return (f->glCheckFramebufferStatus(GL_FRAMEBUFFER) ==
                GL_FRAMEBUFFER_COMPLETE);
}

void cls::create()
{
    quint32 fbo = 0;
    quint32 fboCount = 1;

    f->glGenFramebuffers(fboCount, &fbo);

    f->glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    createAttachments();

    if (isComplete())
    {
        DEBUG("Framebuffer is COMPLETE!!");
    }
    else
    {
        DEBUG("Framebuffer is NOT COMPLETE :( ");
    }

//    f->glBindFramebuffer(GL_FRAMEBUFFER, 0);
//    f->glDeleteFramebuffers(fboCount, &fbo);

}

void cls::createAttachments()
{

    quint32 texCount = 2;

    glGenTextures(texCount, texture);

    glBindTexture(GL_TEXTURE_2D, texture[0]);

    qint32 level = 0;
    qint32 border = 0;
    glTexImage2D(GL_TEXTURE_2D,
                 level, GL_RGB, 800, 600, border,
                 GL_RGB, GL_UNSIGNED_BYTE, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    f->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                              GL_TEXTURE_2D, texture[0], level);


    glBindTexture(GL_TEXTURE_2D, texture[1]);

    glTexImage2D(GL_TEXTURE_2D,
                 level, GL_DEPTH24_STENCIL8, 800, 600, border,
                 GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);

    f->glFramebufferTexture2D(GL_FRAMEBUFFER,
                              GL_DEPTH_STENCIL_ATTACHMENT,
                              GL_TEXTURE_2D, texture[1], level);

}


#include <QLabel>
#include <QVBoxLayout>
void cls::getImage()
{
    static quint8 *buf = new quint8[800*600*3];

    glBindTexture(GL_TEXTURE_2D, texture[0]);

    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, buf);

    QImage img(buf, 800, 600, 800*3, QImage::Format_RGB888);
    img = img.mirrored();

    QWidget *w  = new QWidget;
    QLabel *l = new QLabel;
    l->setPixmap(QPixmap::fromImage(img));
    QVBoxLayout *v= new QVBoxLayout;
    v->addWidget(l);
    w->setLayout(v);
    w->show();
}
