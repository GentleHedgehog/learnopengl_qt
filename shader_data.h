#ifndef SHADER_DATA_H
#define SHADER_DATA_H

#include "shader_program_set.h"

const quint32 SHD_LOCATION_A_POS = 0;
const quint32 SHD_ELEMENT_COUNT_A_POS = 3;
const quint32 SHD_STRIDE_A_POS = 8 * sizeof(GL_FLOAT);
const quint32 SHD_OFFSET_A_POS = 0;

const quint32 SHD_LOCATION_A_COLOR = 1;
const quint32 SHD_ELEMENT_COUNT_A_COLOR = 3;
const quint32 SHD_STRIDE_A_COLOR = 8 * sizeof(GL_FLOAT);
const quint32 SHD_OFFSET_A_COLOR = 3 * sizeof(GL_FLOAT);

const quint32 SHD_LOCATION_A_TEX_COORD = 2;
const quint32 SHD_ELEMENT_COUNT_A_TEX_COORD = 2;
const quint32 SHD_STRIDE_A_TEX_COORD = 8 * sizeof(GL_FLOAT);
const quint32 SHD_OFFSET_A_TEX_COORD = 6 * sizeof(GL_FLOAT);

const QString SHD_MODEL_MATRIX_NAME = "modelMatrix";
const QString SHD_VIEW_MATRIX_NAME = "viewMatrix";
const QString SHD_PROJ_MATRIX_NAME = "projectionMatrix";

extern QString vertexShaderCode;

extern ShaderProgramSet programUsual;

extern ShaderProgramSet programYellow;

#endif // SHADER_DATA_H
