#ifndef MY_GL_WIDGET_H
#define MY_GL_WIDGET_H

#include <QGLWidget>

class MyGlWidget : public QGLWidget
{
public:
    MyGlWidget(QWidget *parent = nullptr);

protected:
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();
};

#endif // MY_GL_WIDGET_H
