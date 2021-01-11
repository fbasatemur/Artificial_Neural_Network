#pragma once

int YPoint(int x, double *weights, int mult= 1);

void MatrixMultiplication(double* mat1, double* mat2, double* mat3, int m1Rows, int m1Cols, int m2Cols);

void Multiplication(double value, double* mat, int matSize);

void Add(double* mat1, int mat1StartIndex, double* mat2, int matSize);

void WeightsUpdate(double* weights, int weightStartIndex, double value, double* input, int inputSize);

int Sgn(double output);

double Sigmoid(double lamda, double net);

double GradientSigmoid(double sigmoidOutput);

void BatchNormalization(Classes* classes, int totalClass);
