#pragma once

struct Sample {
	double x;
	double y;
	double b = 1.0;
};

struct Classes {

	Sample* sample;
	int totalSample;
	int* d;				// expected value

	void AddSample(double clickX, double clickY);
	
	Classes() {
		totalSample = 0;
	}

	~Classes() {
		delete sample;
		delete d;
	}
};