#ifndef SHADER_DATA_H
#define SHADER_DATA_H

#include "shader_helpers.h"


extern sVertexAttributesHelper aPos, aNormal, aTexture;

extern sMatrixHelper<QMatrix4x4> aMatrixHelper;

extern QString vertexShaderCode;
extern QString fragmentShaderCode;


#endif // SHADER_DATA_H
