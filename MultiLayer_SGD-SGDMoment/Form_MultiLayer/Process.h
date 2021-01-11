#pragma once
#include "Sample.h"


int YPoint(int x, double *weights, int mult= 1);

void MatrixMultiplication(const double* mat1,const double* mat2, double* mat3, int m1Rows, int m1Cols, int m2Cols);

void Mean(const Classes* classes, int totalClass, Sample* means);

void StandardD(const Classes* classes, int totalClass, const Sample* means, Sample* standardD);

Classes* BatchNormalization(const Classes* classes, int totalClass, Sample* means, Sample* standardD);

int MaxIndex(double* results, int size);