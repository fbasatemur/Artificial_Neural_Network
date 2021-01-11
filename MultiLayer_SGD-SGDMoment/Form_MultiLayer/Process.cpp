#include <math.h>
#include <float.h>
#include "Process.h"


int YPoint(int x, double* weights, int mult)
{
	return (int)((double)(-1 * (double)mult * weights[2] - weights[0] * x) / (double)(weights[1]));
}

void MatrixMultiplication(const double* mat1, const double* mat2, double* mat3, int m1Rows, int m1Cols, int m2Cols)
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

void Mean(const Classes* classes, int totalClass, Sample* means) {

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

	means->x = totalX / (double)totalSample;
	means->y = totalY / (double)totalSample;
}

void StandardD(const Classes* classes, int totalClass, const Sample* means, Sample* standardD) {

	int totalSample = 0;
	double totalX = 0.0;
	double totalY = 0.0;

	for (int classIndex = 0; classIndex < totalClass; classIndex++)
	{
		for (int sampleIndex = 0; sampleIndex < classes[classIndex].totalSample; sampleIndex++)
		{
			totalX += pow(classes[classIndex].sample[sampleIndex].x - means->x, 2);
			totalY += pow(classes[classIndex].sample[sampleIndex].y - means->y, 2);
		}
		totalSample += classes[classIndex].totalSample;
	}

	standardD->x = sqrt(totalX / (double)totalSample);
	standardD->y = sqrt(totalY / (double)totalSample);
}

Classes* BatchNormalization(const Classes* classes, int totalClass, Sample* means, Sample* standardD) {

	Mean(classes, totalClass, means);
	StandardD(classes, totalClass, means, standardD);

	Classes* batchNormalized = new Classes[totalClass];

	for (int classIndex = 0; classIndex < totalClass; classIndex++) {

		batchNormalized[classIndex].sample = new Sample[classes[classIndex].totalSample];
		batchNormalized[classIndex].totalSample = classes[classIndex].totalSample;
	}
	

	for (int classIndex = 0; classIndex < totalClass; classIndex++)
	{
		for (int sampleIndex = 0; sampleIndex < classes[classIndex].totalSample; sampleIndex++)
		{
			batchNormalized[classIndex].sample[sampleIndex].x = (classes[classIndex].sample[sampleIndex].x - means->x) / standardD->x;
			batchNormalized[classIndex].sample[sampleIndex].y = (classes[classIndex].sample[sampleIndex].y - means->y) / standardD->y;
		}
	}

	return batchNormalized;
}

int MaxIndex(double* results, int size) {

	double currentMax = -DBL_MAX;
	int index = 0;

	for (int i = 0; i < size; i++){
		if (results[i] > currentMax) {
			currentMax = results[i];
			index = i;
		}
	}

	return index;
}



