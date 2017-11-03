QT += core gui
QT += opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
CONFIG   -= debug_and_release debug_and_release_target

#CONFIG += console
#CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    my_gl_widget.cpp \
    ogl_funcs.cpp \
    UsableClass/Macros/macros.cpp

#INCLUDEPATH += \
#            "C:\Program Files (x86)\Microsoft SDKs\Windows\v7.0A\Include\gl"

#LIBS += \
#    "C:\Program Files (x86)\Microsoft SDKs\Windows\v7.0A\Lib\x64\OpenGL32.Lib" \
#    "C:\Program Files (x86)\Microsoft SDKs\Windows\v7.0A\Lib\x64\GlU32.Lib"

HEADERS += \
    my_gl_widget.h \
    ogl_funcs.h \
    UsableClass/Macros/macros.h

#LIBS += \
#    -LC:\Qt\Qt5.7.0\5.7\mingw53_32\lib\libQt5OpenGL.a -lopengl32

LIBS += \
    -lopengl32


#str = $$system("date /t")
#rstr = $$section(str, ., 0, 0)_$$section(str, ., 1, 1)_$$section(str, ., 2, 2)
#message($$rstr)

TARGET = test_#$${rstr}
