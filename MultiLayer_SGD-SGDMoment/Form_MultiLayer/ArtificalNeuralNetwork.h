#pragma once

enum class Activations {Sigmoid, TanH};
enum class TrainType {SGD, SGD_Moment};

class Layer {

private:

	int inputs;
	int neurons;
	long int weightSize;
	double* weight;
	friend class ANeuralNetwork;

	void WeightsUpdateSGD(int neuronIndex, double value, const double* input);

	void WeightsUpdateSGDMoment(int neuronIndex, double value, const double* input, double a, double* lastWeights);

public:
	
	Layer() {}

	void CreateLayer(int inputs);

	int GetNeurons();

	int GetInputs();

	int GetWeightSize();

	double GetWeight(int neuronIndex, int inputIndex);

	double GetWeight(int index);

	void SetWeight(int index, double value);

	~Layer() {
		delete[] weight;
	}

};

class ANeuralNetwork {

private:

	int networkInputs;
	int networkOutputs;
	int totalLayers;

	double momentRate;			// SGD with Moment constant
	double lamda;				// Bipolar Sigmoid constant
	double* learningRate;		// Error Back Propagation Learning Constant

	double** nets;
	double** outputs;
	double** S_ok;
	double** weightsMoment;		// SGD with Moments weights
	double  S_ok_W_kj = 0.0;
	int endIndex;
	double totalError = 0.0;

	TrainType trainType;
	Activations activationType;

	const double* inputSamples;
	const int* destinations;

	double Activation(double net);

	double ActivationDerivative(double net);

	void FeedForward();

	void BackPropagation();

public:

	Layer* layers;
	int totalNeurons = 0;

	ANeuralNetwork(int inputNeurons, int hiddenLayers, int outputNeurons);

	void SetLayerNeurons(int layerIndex, int layerNeurons);

	void SetTrainParameters(double learnRate, double momentRate, TrainType trainType, Activations activationType);

	double GetNetworkOutputs();

	void Create();

	double Train(const double* inputs, const int* destinations);
	
	double* Predict(const double* inputs);

	double RMSE(double totalError, int totalSample);

	~ANeuralNetwork() {

		delete[] layers;
		delete[] learningRate;

		for (int i = 0; i < totalLayers; i++)
		{
			delete nets[i];
			delete outputs[i];
			delete S_ok[i];
			delete weightsMoment[i];
		}
		
		delete[] nets;
		delete[] outputs;
		delete[] S_ok;
		delete[] weightsMoment;		// SGD with Moments weights
	}
};