#include "Sample.h"

void Classes::AddSample(double clickX, double clickY) {

	Sample* oldSamples = sample;
	sample = new Sample[totalSample + 1];

	for (int sampleIndex = 0; sampleIndex < totalSample; sampleIndex++) {
		sample[sampleIndex].x = oldSamples[sampleIndex].x;
		sample[sampleIndex].y = oldSamples[sampleIndex].y;
	}

	sample[totalSample].x = clickX;
	sample[totalSample].y = clickY;
	totalSample++;

	delete oldSamples;
}