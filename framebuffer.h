#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <QObject>
#include "access_to_qt_opengl.h"

class Framebuffer : public AccessToQtOpenGl
{
public:
    Framebuffer();
};

#endif // FRAMEBUFFER_H
