#include <QApplication>

#include "my_gl_widget.h"
#include "ogl_funcs.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    MyGlWidget aMyGlWidget;
//    aMyGlWidget.resize(200, 200);
//    aMyGlWidget.show();


    OGL_funcs f;
    f.show();


    return a.exec();
}
