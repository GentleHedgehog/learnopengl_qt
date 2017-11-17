#include "math_calcs.h"
#include "UsableClass/Macros/macros.h"

#include <QMatrix4x4>
#include <QVector4D>

typedef MathCalcs cls;

namespace  {

QVector4D vec(1.0f, 0.0f, 0.0f, 1.0f);
QMatrix4x4 trans;

}

cls::MathCalcs(QObject *parent) : QObject(parent)
{

//    QVector3D vecForTranslation(1.0f, 1.0f, 0.0f);
//    trans.translate(vecForTranslation);

    QVector3D vecForRotation(0.0f, 0.0f, 1.0f);
    QVector3D vecForScale(0.5f, 0.5f, 0.5f);
    trans.rotate(90, vecForRotation);
    trans.scale(vecForScale);

    vec = trans * vec;

    DEBUG_NM(vec);
    DEBUG_NM(trans);

}
