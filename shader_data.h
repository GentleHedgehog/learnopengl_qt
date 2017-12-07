#ifndef SHADER_DATA_H
#define SHADER_DATA_H

#include "shader_helpers.h"


extern sVboHelper aPos, aColor, aTextureCoord;

extern sMatrixHelper<QMatrix4x4> aMatrixHelper;

extern QString objectColor;
extern QString lightColor;

extern QString vertexShaderCode;
extern QString fragmentShaderCode;


#endif // SHADER_DATA_H
