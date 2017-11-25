#include "camera_setter.h"
#include "shader_data.h"


typedef CameraSetter cls;

cls::CameraSetter(QObject *parent) : QObject(parent)
{

}

void cls::initialize(const QGLContext *curContext,
                     QGLFunctions *funcs,
                     ShaderProgramSet *prog)
{
    context = curContext;
    f = funcs;
    programSet = prog;



    programSet->useProgram();
//    QVector3D vecForModelRotation(1.0f, 0.0f, 0.0f);
//    modelMatrix.rotate(-55.0f, vecForModelRotation);
    viewMatrix.translate(0.0f, 0.0f, 0.0f);
    projectionMatrix.perspective(45.0f,
                                  (screenWidth / screenHeight),
                                  0.1f, 100.0f);
//    projectionMatrix.ortho(0.0f, screenWidth,
//                           0.0f, screenHeight,
//                           0.1f, 100.0f);

    programSet->setUniformMatrixValue(SHD_MODEL_MATRIX_NAME,
                                       modelMatrix.constData());
    programSet->setUniformMatrixValue(SHD_VIEW_MATRIX_NAME,
                                       viewMatrix.constData());
    programSet->setUniformMatrixValue(SHD_PROJ_MATRIX_NAME,
                                       projectionMatrix.constData());



    cameraPosition = QVector3D(0.0f, 0.0f, -3.0f);
    cameraTarget = QVector3D(0.0f, 0.0f, 0.0f);
    cameraDirection = (cameraPosition - cameraTarget).normalized();

    QVector3D worldUpVector(0.0f, 1.0f, 0.0f);
    cameraRight = QVector3D::crossProduct(worldUpVector,
                                       cameraDirection).normalized();

    cameraUp = QVector3D::crossProduct(cameraDirection,
                                       cameraRight);

    lookAtMatrix.lookAt(cameraPosition, cameraTarget, cameraUp);
}
