#ifndef OGL_FUNCS_H
#define OGL_FUNCS_H

#include <QGLWidget>
#include <QGLFunctions>
#include <QTimer>


class OGL_funcs :
        public QGLWidget,
        public QGLFunctions
{
    Q_OBJECT
public:
    OGL_funcs(QWidget *parent = 0);

public slots:
    void doRenderWorkAnywhere();

private:
    QTimer timer;
    QTimer timerForFrameBuffer;


    void createBufObjectsForVertices(bool isFirstTriangle);
    void setAttribs();


    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);
    void textureSettings();
    void generateTextures();

    void keyPressEvent(QKeyEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
};

#endif // OGL_FUNCS_H
