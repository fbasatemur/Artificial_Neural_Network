#include <math.h>
#include "Sample.h"

int YPoint(int x, double* weights, int mult)
{
	return (int)((double)(-1 * (double)mult * weights[2] - weights[0] * x) / (double)(weights[1]));
}

void MatrixMultiplication(double* mat1, double* mat2, double* mat3, int m1Rows, int m1Cols, int m2Cols)
{
	double sum = 0.0;
	for (int row = 0; row < m1Rows; row++)
	{
		for (int col = 0; col < m2Cols; col++)
		{
			sum = 0.0;
			for (int i = 0; i < m1Cols; i++)
			{
				sum += mat1[row * m1Cols + i] * mat2[i * m2Cols + col];
			}
			mat3[row * m2Cols + col] = sum;
		}
	}
}

void Multiplication(double value, double* mat, int matSize)
{
	for (int i = 0; i < matSize; i++)
		mat[i] *= value;

}

void Add(double* mat1, int mat1StartIndex, double* mat2, int matSize)
{
	for (int i = 0; i < matSize; i++)
		mat1[i + mat1StartIndex] += mat2[i];

}

void WeightsUpdate(double* weights, int weightStartIndex, double value, double* input, int inputSize) {

	for (int i = 0; i < inputSize; i++)
		weights[i + weightStartIndex] += value * input[i];
}

int Sgn(double output) {

	return output >= 0.0 ? 1 : -1;
}

double Sigmoid(double lamda, double net) {

	return (2.0 / (1.0 + exp(-lamda * net)) - 1.0);
}

double GradientSigmoid(double sigmoidOutput) {

	return 0.5 * (1.0 - pow(sigmoidOutput, 2));
}


Sample* BatchMean(Classes* classes, int totalClass) {

	double totalX = 0.0;
	double totalY = 0.0;
	int totalSample = 0;

	for (int classIndex = 0; classIndex < totalClass; classIndex++)
	{
		for (int sampleIndex = 0; sampleIndex < classes[classIndex].totalSample; sampleIndex++)
		{
			totalX += classes[classIndex].sample[sampleIndex].x;
			totalY += classes[classIndex].sample[sampleIndex].y;
		}
		totalSample += classes[classIndex].totalSample;
	}

	Sample* tempSample = new Sample;
	tempSample->x = totalX / (double)totalSample;
	tempSample->y = totalY / (double)totalSample;

	return tempSample;
}

Sample* BatchStandardD(Classes* classes, int totalClass, Sample* meanSample) {

	int totalSample = 0;
	double totalX = 0.0;
	double totalY = 0.0;

	for (int classIndex = 0; classIndex < totalClass; classIndex++)
	{
		for (int sampleIndex = 0; sampleIndex < classes[classIndex].totalSample; sampleIndex++)
		{
			totalX += pow(classes[classIndex].sample[sampleIndex].x - meanSample->x, 2);
			totalY += pow(classes[classIndex].sample[sampleIndex].y - meanSample->y, 2);
		}
		totalSample += classes[classIndex].totalSample;
	}

	Sample* tempSample = new Sample;
	tempSample->x = sqrt(totalX / (double)totalSample);
	tempSample->y = sqrt(totalY / (double)totalSample);

	return tempSample;
}

void BatchNormalization(Classes* classes, int totalClass) {

	Sample* meanSample = BatchMean(classes, totalClass);
	Sample* sdSample = BatchStandardD(classes, totalClass, meanSample);

	for (int classIndex = 0; classIndex < totalClass; classIndex++)
	{
		for (int sampleIndex = 0; sampleIndex < classes[classIndex].totalSample; sampleIndex++)
		{
			classes[classIndex].sample[sampleIndex].x = (classes[classIndex].sample[sampleIndex].x - meanSample->x) / sdSample->x;
			classes[classIndex].sample[sampleIndex].y = (classes[classIndex].sample[sampleIndex].y - meanSample->y) / sdSample->y;
		}
	}
}
